#pragma once
#include <string>
#include <glad/glad.h>

class Shader {
public:
    Shader(const std::string &vertPath, const std::string &fragPath);

    void use() const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const float *mat) const;

    GLuint id;
private:

    static std::string loadSource(const std::string &filePath);
    static GLuint compileShader(const std::string &source, GLenum type);
    static GLuint createProgram(const std::string &vertPath, const std::string &fragPath);
};
