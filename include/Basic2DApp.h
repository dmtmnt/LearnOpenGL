#pragma once

#include <Camera.h>
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>


class VertexLayout;

class Basic2DApp
{
public:
    Basic2DApp(const char* window_name, const std::size_t width, const std::size_t height);
     ~Basic2DApp();
        
     void renderLoop();
private:
    bool initialize(const char* window_name, const std::size_t width, const std::size_t height);
    void getVertexData(std::vector<float>& vertices);
    void creteVertexLayout(VertexLayout& vertexLayout);
    void processInput(GLFWwindow *window, glm::mat4& transformMat, std::array<int, 10>& m_enableDrawings);

private:
    GLFWwindow* m_window = NULL;
};





