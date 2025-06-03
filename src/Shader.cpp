#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string &vertPath, const std::string &fragPath) {
	id = createProgram(vertPath, fragPath);
}

void Shader::use() const {
	glUseProgram(id);
}

void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const float *mat) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, mat);
}

std::string Shader::loadSource(const std::string &filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Failed to open shader file: " << filePath << "\n";
		exit(1);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

GLuint Shader::compileShader(const std::string &source, GLenum type) {
	GLuint shader = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
		std::cerr << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
			  << " shader compile error:\n" << infoLog << "\n";
		exit(1);
	}

	return shader;
}

GLuint Shader::createProgram(const std::string &vertPath, const std::string &fragPath) {
	std::string vertSrc = loadSource(vertPath);
	std::string fragSrc = loadSource(fragPath);

	GLuint vertShader = compileShader(vertSrc, GL_VERTEX_SHADER);
	GLuint fragShader = compileShader(fragSrc, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "Shader program link error:\n" << infoLog << "\n";
		exit(1);
	}

	return program;
}
