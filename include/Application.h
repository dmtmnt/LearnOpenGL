#pragma once
#include <Camera.h>
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>


//Application is just a helper class that abstracts away the creation of the context, the window, and the user inputs.
class VertexLayout;
class VertexBuffer;

class Application
{
public:
    Application(const char* window_name, const std::size_t width, const std::size_t height);
     ~Application();
    
     void renderLoop();
    
private:
    bool initialize(const char* window_name, const std::size_t width, const std::size_t height) ;
    void processInput(GLFWwindow *window, glm::mat4& transformMat, std::array<int, 10>& m_enableDrawings);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void cretePlaneLayout(VertexLayout& result);
private:
    GLFWwindow* m_window = NULL;
    static Camera m_camera;
    static CameraData m_CameraUtil;
};





