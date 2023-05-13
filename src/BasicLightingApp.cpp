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


CameraData BasicLightingApp::m_CameraUtil = CameraData();
Camera BasicLightingApp::m_camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

BasicLightingApp::BasicLightingApp(const char* window_name, const std::size_t width, const std::size_t height)
{
    initialize(window_name, width, height);
}

BasicLightingApp::~BasicLightingApp()
{
}

bool BasicLightingApp::initialize(const char* window_name, const std::size_t width, const std::size_t height)
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
    
    
    glfwSetCursorPosCallback(m_window, BasicLightingApp::mouse_callback);
    glfwSetScrollCallback(m_window, BasicLightingApp::scroll_callback);
    
    
    if(!m_window)
    {
        assert(0 && "Failed to create window and context");
        return true;
    }
    
    return false;
}


void BasicLightingApp::renderLoop()
{
    // shader
    ShaderProgram lightingSource("../../resources/shader/v4_light_source.vs", "../../resources/shader/v4_light_source.fs");

    ShaderProgram lightingShader("../../resources/shader/v4_basic_lighting.vs", "../../resources/shader/v4_basic_lighting.fs");
    ShaderProgram lightingShaderAmbient("../../resources/shader/v5_basic_lighting.vs", "../../resources/shader/v5_ambinet_light.fs");
    ShaderProgram lightingShaderDifuse("../../resources/shader/v5_basic_lighting.vs", "../../resources/shader/v5_difuse_light.fs");
    ShaderProgram lightingShaderSpecular("../../resources/shader/v5_basic_lighting.vs", "../../resources/shader/v5_specular_light.fs");
    ShaderProgram lightingShaderPhong("../../resources/shader/v5_basic_lighting.vs", "../../resources/shader/v5_phong_light.fs");
    
    // textures
    TextureData texture_wall("../../resources/textures/wall.jpg");
    
    std::vector<float> vertices;
    getCubeWithNormals(vertices);

    VertexLayout lightedCubeLayout;
    creteCubVertexWithNormalsLayout(lightedCubeLayout);
    
    VertexBuffer cubeVao;
    const size_t stride = 6;
    cubeVao.create(&vertices[0], &lightedCubeLayout, vertices.size() * sizeof(float), stride);
    
    VertexBuffer lightVao;
    lightVao.create(&vertices[0], &lightedCubeLayout, vertices.size() * sizeof(float), stride);
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection  = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    float angle = 20.0f;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    
    std::array<int, 10> enableDrawings{};
    
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(m_window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        m_CameraUtil.deltaTime = currentFrame - m_CameraUtil.lastFrame;
        m_CameraUtil.lastFrame = currentFrame;
        
        processInput(m_window, model, enableDrawings);
        
        // clear window
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        glm::mat4 view = m_camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(m_camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
        // light source
        {
            lightingSource.bind();
            glm::mat4 lamp_model = glm::mat4(1.0f);
            lamp_model = glm::translate(lamp_model, lightPos);
            lamp_model = glm::scale(lamp_model, glm::vec3(0.2f));
            lightingShader.set_uniform("model", lamp_model);
            lightingShader.set_uniform("view", view);
            lightingShader.set_uniform("projection", projection);
            
            lightVao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // basic lighted object
        if(enableDrawings[0])
        {
            // lighted cube
            lightingShader.bind();
            lightingShader.set_uniform("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            lightingShader.set_uniform("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));

            lightingShader.set_uniform("model", glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)));
            lightingShader.set_uniform("view", view);
            lightingShader.set_uniform("projection", projection);
            
            cubeVao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // ambient lighted object
        if(enableDrawings[1])
        {
            // lighted cube
            lightingShaderAmbient.bind();
            lightingShaderAmbient.set_uniform("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            lightingShaderAmbient.set_uniform("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));

            lightingShaderAmbient.set_uniform("model", glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)));
            lightingShaderAmbient.set_uniform("view", view);
            lightingShaderAmbient.set_uniform("projection", projection);
            
            cubeVao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // difuse lighted object
        if(enableDrawings[2])
        {
            lightingShaderDifuse.bind();
            lightingShaderDifuse.set_uniform("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            lightingShaderDifuse.set_uniform("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
            lightingShaderDifuse.set_uniform("lightPos", lightPos);
            
            lightingShaderDifuse.set_uniform("model", glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)));
            lightingShaderDifuse.set_uniform("view", view);
            lightingShaderDifuse.set_uniform("projection", projection);
            
            cubeVao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // specular
        if(enableDrawings[3])
        {
            lightingShaderSpecular.bind();
            lightingShaderSpecular.set_uniform("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            lightingShaderSpecular.set_uniform("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
            lightingShaderSpecular.set_uniform("lightPos", lightPos);
            lightingShaderSpecular.set_uniform("viewPos", m_camera.Position);
            
            lightingShaderSpecular.set_uniform("model", glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)));
            lightingShaderSpecular.set_uniform("view", view);
            lightingShaderSpecular.set_uniform("projection", projection);
            
            cubeVao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
                                       
       // phong
       if(enableDrawings[4])
       {
           lightingShaderPhong.bind();
           lightingShaderPhong.set_uniform("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
           lightingShaderPhong.set_uniform("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
           lightingShaderPhong.set_uniform("lightPos", lightPos);
           lightingShaderPhong.set_uniform("viewPos", m_camera.Position);
           
           lightingShaderPhong.set_uniform("model", glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)));
           lightingShaderPhong.set_uniform("view", view);
           lightingShaderPhong.set_uniform("projection", projection);
           
           cubeVao.bind();
           glDrawArrays(GL_TRIANGLES, 0, 36);
       }

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    
    cubeVao.clear();
    lightVao.clear();
    glfwTerminate();
}

void BasicLightingApp::creteCubVertexWithNormalsLayout(VertexLayout& vertexLayout)
{
    // position attribute
    std::size_t location = 0, offset = 0, number_of_floats = 3;
    VertexAttribute attr = vertexLayout.CreateVertexAttribute(AttributeType::Position, location, offset, number_of_floats);
    vertexLayout.AddVertexAttribute(attr);
    
    // normal attribute
    location = 1; offset = 3; number_of_floats = 3;
    attr = vertexLayout.CreateVertexAttribute(AttributeType::Texture, location, offset, number_of_floats);
    vertexLayout.AddVertexAttribute(attr);
}

void BasicLightingApp::processInput(GLFWwindow *window, glm::mat4& transformMat, std::array<int, 10>& enableDrawings)
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

void BasicLightingApp::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

void BasicLightingApp::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    m_camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


void BasicLightingApp::getCubeWithNormals(std::vector<float>& vertices)
{
    vertices = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
}
