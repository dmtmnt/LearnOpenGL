#include <Basic3DApp.h>
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

CameraData Basic3DApp::m_CameraUtil = CameraData();
Camera Basic3DApp::m_camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

Basic3DApp::Basic3DApp(const char* window_name, const std::size_t width, const std::size_t height)
{
    initialize(window_name, width, height);
}

Basic3DApp::~Basic3DApp()
{
}

bool Basic3DApp::initialize(const char* window_name, const std::size_t width, const std::size_t height)
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
    
    
    glfwSetCursorPosCallback(m_window, Basic3DApp::mouse_callback);
    glfwSetScrollCallback(m_window, Basic3DApp::scroll_callback);
    
    
    if(!m_window)
    {
        assert(0 && "Failed to create window and context");
        return true;
    }
    
    return false;
}

void Basic3DApp::renderLoop()
{
    // shaders
    ShaderProgram shader("../../resources/shader/v2_shader.vs","../../resources/shader/v2_shader.fs");
    
    // textures
    TextureData texture_wall("../../resources/textures/wall.jpg");
    TextureData texture_penguin("../../resources/textures/penguin.png");
    TextureData texture_lion("../../resources/textures/lion.png");
    
    // vbo, vao, ebo
    std::vector<float> vertices;
    getCubeVertexData(vertices);
    
    VertexLayout vertexLayout;
    creteCubVertexLayout(vertexLayout);
    
    VertexBuffer buffer;
    const std::size_t stride = 5;
    buffer.create(&vertices[0], &vertexLayout, vertices.size() * sizeof(float), stride);
    
  

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    std::array<int, 10> enableDrawings{};
    glEnable(GL_DEPTH_TEST);
    
    while(!glfwWindowShouldClose(m_window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        m_CameraUtil.deltaTime = currentFrame - m_CameraUtil.lastFrame;
        m_CameraUtil.lastFrame = currentFrame;
        
        processInput(m_window, model, enableDrawings);
        
        // clear window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // update only model matrix
        if(enableDrawings[0])
        {
            shader.bind();
            shader.set_uniform("model", model);
            shader.set_uniform("view", view);
            shader.set_uniform("projection", projection);
            
            texture_wall.bind(0);
            buffer.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
        }
        // update only model matrix
        if(enableDrawings[1])
        {
            shader.bind();
            shader.set_uniform("model", model);
            shader.set_uniform("view", view);
            shader.set_uniform("projection", projection);
            
            texture_wall.bind(0);
            buffer.bind();
            
            std::vector<glm::vec3> cubePositions;
            getCubePostions(cubePositions);
            for(unsigned int i = 1; i < cubePositions.size(); i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                shader.bind();
                shader.set_uniform("model", model);
                shader.set_uniform("projection", projection);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        // update view matrix
        if(enableDrawings[2])
        {
            shader.bind();
            
            glm::mat4 view = m_camera.GetViewMatrix();
            shader.set_uniform("view", view);
            shader.set_uniform("projection", projection);
            
            std::vector<glm::vec3> cubePositions;
            getCubePostions(cubePositions);
            for(unsigned int i = 1; i < cubePositions.size(); i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                shader.set_uniform("model", model);
                
                texture_wall.bind(0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        if(enableDrawings[3])
        {
            shader.bind();
            
            glm::mat4 view = m_camera.GetViewMatrix();
            shader.set_uniform("view", view);
            
            glm::mat4 model2 = glm::mat4(1.0f);
            model2 = glm::translate(model, glm::vec3(-0.5, 0.3f, 0.0f));
            shader.set_uniform("model", model2);
            
            texture_penguin.bind(0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
            model2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.0f, 0.0f));
            shader.set_uniform("model", model2);
            
            texture_lion.bind(0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    
    buffer.clear();
    glfwTerminate();
}

void Basic3DApp::creteCubVertexLayout(VertexLayout& vertexLayout)
{
    // position attribute
    std::size_t location = 0, offset = 0, number_of_floats = 3;
    VertexAttribute attr = vertexLayout.CreateVertexAttribute(AttributeType::Position, location, offset, number_of_floats);
    vertexLayout.AddVertexAttribute(attr);
    
    // texture attribute
    location = 1; offset = 3; number_of_floats = 2;
    attr = vertexLayout.CreateVertexAttribute(AttributeType::Texture, location, offset, number_of_floats);
    vertexLayout.AddVertexAttribute(attr);
}

void Basic3DApp::processInput(GLFWwindow *window, glm::mat4& transformMat, std::array<int, 10>& enableDrawings)
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
    
    // update camera position
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.ProcessKeyboard(FORWARD, m_CameraUtil.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.ProcessKeyboard(BACKWARD, m_CameraUtil.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera.ProcessKeyboard(LEFT, m_CameraUtil.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera.ProcessKeyboard(RIGHT, m_CameraUtil.deltaTime);
}

void Basic3DApp::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    if (m_CameraUtil.firstMouse)
    {
        m_CameraUtil.lastX = xpos;
        m_CameraUtil.lastY = ypos;
        m_CameraUtil.firstMouse = false;
    }
    
    float xoffset =xpos - m_CameraUtil.lastX;
    float yoffset = m_CameraUtil.lastY - ypos; // reversed since y-coordinates go from bottom to top
    
    m_CameraUtil.lastX = xpos;
    m_CameraUtil.lastY = ypos;
    
    m_camera.ProcessMouseMovement(xoffset, yoffset);
}

void Basic3DApp::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    m_camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


void Basic3DApp::getCubeVertexData(std::vector<float>& vertices)
{
    vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
}

void Basic3DApp::getCubePostions(std::vector<glm::vec3>& pos)
{
    pos = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
}
