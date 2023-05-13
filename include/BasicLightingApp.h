#pragma once

#include <Camera.h>
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <array>

class VertexLayout;

class BasicLightingApp
{
public:
    BasicLightingApp(const char* window_name, const std::size_t width, const std::size_t height);
     ~BasicLightingApp();
     
    
     void renderLoop();
 
private:
    bool initialize(const char* window_name, const std::size_t width, const std::size_t height);
    void getCubeWithNormals(std::vector<float>& vertices);
    void creteCubVertexWithNormalsLayout(VertexLayout& vertexLayout);
    void processInput(GLFWwindow *window, glm::mat4& transformMat, std::array<int, 10>& m_enableDrawings);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
    GLFWwindow* m_window = NULL;
    static Camera m_camera;
    static CameraData m_CameraUtil;
};





