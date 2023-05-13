#pragma once

#include <Camera.h>
#include <Texture.h>
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>


class Car
{
public:
    Car(float lenght, float speed, float x, float y, int textureIndex)
    : m_lenght(lenght), m_speed(speed), m_x(0), m_y(0), m_textureIndex(textureIndex)
    {
        m_transform = glm::translate(glm::mat4(1), glm::vec3(x, y, 0.0f));
    };
    void getPosition(float& x, float& y) { x = m_x; y = m_y; }
    int getTextureIndex() const { return m_textureIndex; }
    void setTextureIndex(int val)  {  m_textureIndex = val; }
    float getPositionX() { return m_x; }
    float setPositionX(float x) { m_x = x; }
    float getSpeed() { return m_speed; }
    void setPosition(float x, float y) { m_x = x; m_y = y; }
    const glm::mat4& Move()
    {
        // std::cout << " 1 Car = " << m_transform[3].x  << std::endl;
        m_transform = glm::translate(m_transform, glm::vec3(m_speed, 0.0f, 0.0f));
        // std::cout << " 2 Car = " << m_transform[3].x  << std::endl;
        return  m_transform;
    }
    
    void setTransform(const glm::mat4& pos ) {  m_transform = pos; }
private:
    float m_lenght = 0.f;
    float m_speed = 1.f;
    float m_x = 0.f;
    float m_y = 0.f;
    int m_textureIndex = 0;
    glm::mat4 m_transform;
};
typedef std::shared_ptr<Car> CarPtr;
typedef std::vector<CarPtr> CarPtrVec;

class Lane
{
public:
    Lane(const glm::mat4& lanePos)
    {
        m_lanePos = lanePos;
    }
    
    const glm::mat4& getLanePos() const { return m_lanePos; }
    
    void AddCar(const CarPtr& car)
    {
        m_carVec.push_back(car); 
    }
    
    const CarPtrVec& getCars() const
    {
        return m_carVec;
    }
    
    bool CheckPos(int pos)
    {
       // return cars[pos];
    }
private:

    std::vector<CarPtr> m_carVec;
    glm::mat4 m_lanePos;
};
typedef std::shared_ptr<Lane> LanePtr;
typedef std::vector<LanePtr> LanePtrVec;


class VertexLayout;

class BasicGameApp
{
public:
    BasicGameApp(const char* window_name, const std::size_t width, const std::size_t height);
     ~BasicGameApp();
        
     void renderLoop();

private:
    void createLanes(LanePtrVec& lanes, int numCarTextures) const;
    void createCarTextures(TextureVec& textures) const;
    
    bool initialize(const char* window_name, const std::size_t width, const std::size_t height);
    void creteVertexLayout(VertexLayout& vertexLayout);
    void processInput(GLFWwindow *window, glm::mat4& transformMat, std::array<int, 10>& m_enableDrawings);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
    GLFWwindow* m_window = NULL;
    static Camera m_camera;
    static CameraData m_CameraUtil;
};





