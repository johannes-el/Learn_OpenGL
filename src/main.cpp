#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./stb_image.h"

std::string GetShaderSource(const std::string file_name) {
  std::string result = "";
  std::ifstream file(file_name);

  if (!file.is_open()) {
    std::cerr << "Could not open shader file!" << std::endl;
    exit(1);
  }

  std::string line;
  while (std::getline(file, line)) {
    result += line + '\n';
  }

  return result;
}

GLuint CompileShader(const std::string& shaderSource, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const char* source = shaderSource.c_str();
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment") 
                  << " Shader Compilation Failed: " << infoLog << std::endl;
        exit(1);
    }
    return shader;
}

GLuint CreateShaderProgram(const std::string& vertShaderPath, const std::string& fragShaderPath) {
    std::string vertShaderSource = GetShaderSource(vertShaderPath);
    std::string fragShaderSource = GetShaderSource(fragShaderPath);

    GLuint vertShader = CompileShader(vertShaderSource, GL_VERTEX_SHADER);
    GLuint fragShader = CompileShader(fragShaderSource, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);

    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program Linking Failed: " << infoLog << std::endl;
        exit(1);
    }

    return program;
}

int main() {
  constexpr int SCREEN_WIDTH = 800;
  constexpr int SCREEN_HEIGHT = 600;

  if (!glfwInit()) {
    std::cerr << "Could not initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Learn OpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Window creation failed" << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  std::vector<GLfloat> vertices = {
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f,  0.5f, 0.0f
  };

  std::vector<GLuint> indices = {
    0, 2, 1,
    1, 2, 3
  };

  GLuint program = CreateShaderProgram("./shaders/vert.glsl", "./shaders/frag.glsl");

  GLuint VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  double prev_s = glfwGetTime();
  double title_countdown_s = 0.1;

  int widthImg, heightImg, numColCh;
  unsigned char* bytes = stbi_load("./textures/github.png", &widthImg, &heightImg, &numColCh, 0);

  GLuint texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(bytes);
  glBindTexture(GL_TEXTURE_2D, 0);

  /*
  GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
  shaderProgram.Activate();
  glUniform1i(tex0Uni, 0);
  */

  while (!glfwWindowShouldClose(window)) {

    double curr_s = glfwGetTime();
    double elapsed_s = curr_s - prev_s;
    prev_s = curr_s;

    title_countdown_s -= elapsed_s;
    if (title_countdown_s <= 0.0 && elapsed_s > 0.0) {
      double fps = 1.0 / elapsed_s;
      char tmp[256];
      sprintf(tmp, "FPS %.2lf", fps);
      glfwSetWindowTitle(window, tmp);
      title_countdown_s = 0.1;
    }

    glfwPollEvents();
    glClearColor(0.5f, 0.7f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glDeleteTextures(1, &texture);

  glfwTerminate();
  return 0;
}
