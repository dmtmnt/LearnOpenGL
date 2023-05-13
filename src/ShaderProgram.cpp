#include <ShaderProgram.h>
#include <fstream>
#include <string>
#include <sstream>
#include <glew.h>
#include <glfw3.h>

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
{
    create(vertexPath, fragmentPath);
}

ShaderProgram::~ShaderProgram()
{
    
}

void ShaderProgram::bind() const
{
    glUseProgram(m_shaderProgram);
}

// Create with vertex shader and frag shader files.
bool ShaderProgram::create(const char* vertexPath, const char* fragmentPath)
{
    // 1. get vertex, fragment shader code
    std::string vertexShaderCode, fragmentShaderCode;
    getShaderCode(vertexPath, fragmentPath, vertexShaderCode, fragmentShaderCode);
      
    const char* vShaderCode = vertexShaderCode.c_str();
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    {
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            return 0;
        }
    }
    
    // fragment shader
    const char* fShaderCode = fragmentShaderCode.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    {
        int  success;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            return 0;
        }
    }
    
    // link shaders in a program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    {
        int  success;
        char infoLog[512];
        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
            return 0;
        }
    }
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return true;
}

void ShaderProgram::getShaderCode(const char* vertexPath, const char* fragmentPath,  std::string& vShaderCode,  std::string& fShaderCode)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    vShaderCode = vertexCode;
    fShaderCode = fragmentCode;
}

void ShaderProgram::set_uniform(const std::string &name, float x, float y, float z)
{
    int uniform_location = glGetUniformLocation(m_shaderProgram, name.c_str());
    glUniform4f(uniform_location, x, y, z, 1.0f);
}


void ShaderProgram::set_uniform(const std::string &name, const glm::vec3& value)
{
    int uniform_location = glGetUniformLocation(m_shaderProgram, name.c_str());
    glUniform3fv(uniform_location, 1, glm::value_ptr(value) );
}

void ShaderProgram::set_uniform(const std::string &name, const glm::mat4& value)
{
    int uniform_location = glGetUniformLocation(m_shaderProgram, name.c_str());
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value));
}
