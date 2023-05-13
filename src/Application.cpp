#include <Application.h>
#include <BasicLightingApp.h>
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

CameraData Application::m_CameraUtil = CameraData();
Camera Application::m_camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

Application::Application(const char* window_name, const std::size_t width, const std::size_t height)
{
    initialize(window_name, width, height);
}

Application::~Application()
{
}

bool Application::initialize(const char* window_name, const std::size_t width, const std::size_t height)
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
    
    
    glfwSetCursorPosCallback(m_window, Application::mouse_callback);
    glfwSetScrollCallback(m_window, Application::scroll_callback);
    
    
    if(!m_window)
    {
        assert(0 && "Failed to create window and context");
        return true;
    }
    
    return false;
}

void Application::cretePlaneLayout(VertexLayout& layout)
{
    // position attribute
    std::size_t location = 0, offset = 0, number_of_floats = 3;
    VertexAttribute attr = layout.CreateVertexAttribute(AttributeType::Position, location, offset, number_of_floats);
    layout.AddVertexAttribute(attr);
    
//    // texture attribute
//    location = 1; offset = 3; number_of_floats = 2;
//    attr = layout.CreateVertexAttribute(AttributeType::Texture, location, offset, number_of_floats);
//    layout.AddVertexAttribute(attr);
}

void Application::renderLoop()
{
    ShaderProgram shader("../../resources/shader/v1_shader.vs","../../resources/shader/v1_shader.fs");
    TextureData texture_wall("../../resources/textures/wall.jpg");
    
    std::vector<float> planeVertices1 = {
        // positions
        -0.5f, 0.0f,  0.0f, // left
         0.5f, 0.0f,  0.0f, // right
         0.0f, 0.5f,  0.0f, // to
    };
    
    std::vector<float> planeVertices2= {
        // positions
         46932507.f ,  0.0f, // left
        9387641.f, 52663840.f, 0.0f, // to
        79466554.f, 52983760.f, 0.0f,
    };
    
    std::vector<float> planeVertices = {
        // positions
        70541473.5f, 46386715.733568549f,  0.0f, // left
        70546216.482497916f, 46386437.749460131f, 0.0f, // to
        70539400.602073327f, 46385541.551809542f, 0.0f,
    };
    
    for(int i = 0; i < planeVertices.size(); ++i)
    {
        if(planeVertices[i] >= 385541.f)
        planeVertices[i] -= 385541;
    }
    
   // tn::foundation::Mercator(134210000, 134210000),
   //tn::foundation::Mercator(134217728, 134220000)
    
    VertexLayout planeLayout;
    cretePlaneLayout(planeLayout);
    
    VertexBuffer planeVao;
    const std::size_t stride = 3;
    planeVao.create(&planeVertices[0], &planeLayout, planeVertices.size() * sizeof(float), stride);
    
    std::array<int, 10> enableDrawings{};
    glEnable(GL_DEPTH_TEST);
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
 //   model = glm::translate(model, glm::vec3(0.5f, 0, 0));
//    view = glm::translate(model, glm::vec3(100, 0, 0));

    glm::vec3 cameraPos   = glm::vec3(planeVertices[0], planeVertices[1],  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
    
    m_CameraUtil.cameraPos = cameraPos;
    m_CameraUtil.cameraFront = cameraFront;
 
    m_camera.SetPositionUp(m_CameraUtil.cameraPos, m_CameraUtil.cameraUp);
    
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
        
        glm::mat4 projection = glm::perspective(glm::radians(m_camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = m_camera.GetViewMatrix();
        //glm::mat4 model = glm::mat4(1.0f);
        
        shader.bind();
        shader.set_uniform("model", model);
        shader.set_uniform("view", view);
        shader.set_uniform("projection", projection);
            
        planeVao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
            
        //glDrawArrays(GL_LINES, 0, 2);
        
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    
    planeVao.clear();
    glfwTerminate();

}


void Application::processInput(GLFWwindow *window, glm::mat4& transformMat, std::array<int, 10>& m_enableDrawings)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float trans_x = 0.f, trans_y = 0.f, delta = .01f;
    
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        trans_x += delta;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        trans_x -= delta;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        trans_y -= delta;
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        trans_y += delta;
    }
    
    transformMat = glm::translate(transformMat, glm::vec3(trans_x, trans_y, 0.0f));
    
    float angle_in_degrees = 0.1f;
    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        transformMat = glm::rotate(transformMat, angle_in_degrees, glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        transformMat = glm::rotate(transformMat, angle_in_degrees, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    
    if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        m_enableDrawings[0] = 1;
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        m_enableDrawings[1] = 1;
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        m_enableDrawings[2] = 1;
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        m_enableDrawings[3] = 1;
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        m_enableDrawings[4] = 1;
    if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        m_enableDrawings[5] = 1;
    
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

void Application::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // m_camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

