#include <Basic2DApp.h>
#include <ShaderProgram.h>
#include <VertexBuffer.h>
#include <VertexLayout.h>
#include <Texture.h>
#include <IndexBuffer.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

Basic2DApp::Basic2DApp(const char* window_name, const std::size_t width, const std::size_t height)
{
    initialize(window_name, width, height);
}

Basic2DApp::~Basic2DApp()
{
}

bool Basic2DApp::initialize(const char* window_name, const std::size_t width, const std::size_t height)
{
    if(!glfwInit())
    {
        assert(0 && "GLFW nbot initialized");
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    m_window = glfwCreateWindow(width, height, window_name, NULL, NULL);
    if (m_window == NULL)
    {
        assert(0 && "Failed to create GLFW window");
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_window);
    glewInit();
    
    if(!m_window)
    {
        assert(0 && "Failed to create window and context");
        return true;
    }
    
    return false;
}


void Basic2DApp::renderLoop()
{
    // shaders
    ShaderProgram shaderUniform("../../resources/shader/vertex_transform_uniform.vs","../../resources/shader/vertex_transform_uniform.fs");
    ShaderProgram shader_v1("../../resources/shader/v1_shader.vs","../../resources/shader/v1_shader.fs");
    ShaderProgram shader_v2("../../resources/shader/v2_shader.vs","../../resources/shader/v2_shader.fs");


    // textures
    TextureData texture_wall("../../resources/textures/wall.jpg");
    TextureData texture_penguin("../../resources/textures/penguin.png");
    TextureData texture_lion("../../resources/textures/lion.png");
    
    // transform matrices
    glm::mat4 transformMat0 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.5f, 0.0f));
    glm::mat4 transformMat1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));
    glm::mat4 transformMat2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));
    glm::mat4 transformMat3 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));

    std::vector<float> vertices;
    getVertexData(vertices);

    VertexLayout vertexLayout;
    creteVertexLayout(vertexLayout);

    VertexBuffer buffer;
    const std::size_t stride = 3;
    buffer.create(&vertices[0], &vertexLayout, vertices.size() * sizeof(float), stride);
   
    std::array<int, 10> enable;

    glm::mat4 transformMatKeyPress = transformMat0;
    while(!glfwWindowShouldClose(m_window))
    {
        processInput(m_window, transformMatKeyPress, enable);
        
        // clear window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shaderUniform.bind();
        buffer.bind();
       
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    buffer.clear();

    glfwTerminate();
}


void Basic2DApp::processInput(GLFWwindow *window, glm::mat4& transformMat, std::array<int, 10>& enableDrawings)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float trans_x = 0.f, trans_y = 0.f, delta = .01f;

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        trans_x += delta;
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        trans_x -= delta;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        trans_y -= delta;
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        trans_y += delta;
    
    transformMat = glm::translate(transformMat, glm::vec3(trans_x, trans_y, 0.0f));

    float angle_in_degrees = 0.1f;
    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        transformMat = glm::rotate(transformMat, angle_in_degrees, glm::vec3(1.0f, 0.0f, 0.0f));
    if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        transformMat = glm::rotate(transformMat, angle_in_degrees, glm::vec3(0.0f, 1.0f, 0.0f));

    if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
     {
        enableDrawings.fill(0);
        enableDrawings[0] = 1;
     }
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        enableDrawings.fill(0);
        enableDrawings[1] = 1;
    }
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        enableDrawings.fill(0);
        enableDrawings[2] = 1;
    }
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        enableDrawings.fill(0);
        enableDrawings[3] = 1;
    }
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        enableDrawings.fill(0);
        enableDrawings[4] = 1;
    }
    if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        enableDrawings.fill(0);
        enableDrawings[5] = 1;
    }
}

void Basic2DApp::getVertexData(std::vector<float>& vertices)
{
    // application data
    vertices = {
        // first triangle
          -0.5f, -0.5f, 0.0f,
           0.5f, -0.5f, 0.0f,
           0.0f,  0.5f, 0.0f
    };
}

void Basic2DApp::creteVertexLayout(VertexLayout& vertexLayout)
{
    // position attribute
    std::size_t location = 0, offset = 0, number_of_floats = 3;
    VertexAttribute attr = vertexLayout.CreateVertexAttribute(AttributeType::Position, location, offset, number_of_floats);
    vertexLayout.AddVertexAttribute(attr);
    
//    // texture attribute
//    location = 1; offset = 3; number_of_floats = 2;
//    attr = vertexLayout.CreateVertexAttribute(AttributeType::Texture, location, offset, number_of_floats);
//    vertexLayout.AddVertexAttribute(attr);
}
