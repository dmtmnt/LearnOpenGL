#pragma once
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram
{
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    ~ShaderProgram();

    void bind() const;
    
    void set_uniform(const std::string &name, float x, float y, float z);
    void set_uniform(const std::string &name, const glm::mat4& value);
    void set_uniform(const std::string &name, const glm::vec3& value);
private:
    bool create(const char* vertexPath, const char* fragmentPath);
    void getShaderCode(const char* vertexPath, const char* fragmentPath, std::string& vShaderCode, std::string& fShaderCode);
    unsigned int m_shaderProgram;
};

