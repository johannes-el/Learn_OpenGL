#include <iostream>
#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    -0.5f, -0.5f, 0.0f,  // vertex 1
    0.5f, -0.5f, 0.0f,   // vertex 2
    0.0f,  0.5f, 0.0f    // vertex 3
  };

  GLuint program = CreateShaderProgram("./shaders/vert.glsl", "./shaders/frag.glsl");

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClearColor(0.5f, 0.7f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
