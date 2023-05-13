#include <BasicGameApp.h>
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
#include <thread>
#include <list>
#include <random>
#include <chrono>


CameraData BasicGameApp::m_CameraUtil = CameraData();
Camera BasicGameApp::m_camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

BasicGameApp::BasicGameApp(const char* window_name, const std::size_t width, const std::size_t height)
{
    initialize(window_name, width, height);
}

BasicGameApp::~BasicGameApp()
{
}

bool BasicGameApp::initialize(const char* window_name, const std::size_t width, const std::size_t height)
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
    
    glfwSetCursorPosCallback(m_window, BasicGameApp::mouse_callback);
    glfwSetScrollCallback(m_window, BasicGameApp::scroll_callback);
    
    if(!m_window)
    {
        assert(0 && "Failed to create window and context");
        return true;
    }
    
    return false;
}

void BasicGameApp::createLanes(LanePtrVec& lanes, int numCarTextures) const
{
    lanes.clear();
    
    float car_lenght = 0.4f, car_height = 0.2f;
    float cmin_x =  -1.0f, cmax_x = cmin_x + car_lenght;
    float cmin_y =   0.0f, cmax_y = car_height;
    
    float car_speed = 0.01f;
 
    const float lane_height = 0.20f;
    const float total_height = 2.f;
    int num_lanes = total_height / lane_height - 2 ; // 2 = start and end
    
  
    std::vector<glm::mat4> lane_transforms;
 
   
    for(int i = 0; i < num_lanes/2; i++)
    {
        float lane_y = lane_height/2 + i* lane_height;
        
        lane_transforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, lane_y, 0.0f)));
        lane_transforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -lane_y, 0.0f)));
    }
    
    const float car_max_speed = 0.03f, car_min_speed = 0.01f;
    for(int i = 0; i < lane_transforms.size(); i++)
    {
        auto lanePtr = std::make_shared<Lane>(lane_transforms[i]);
        float speed = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/car_max_speed);
        if(speed < car_min_speed )  speed = car_min_speed;
        int texture = rand()%(numCarTextures-1);
        auto carPtr = std::make_shared<Car>(car_lenght, speed , cmin_x, cmin_y, texture);
    
        glm::mat4 transformCar = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, lane_transforms[i][3].y, 0.0f));
        carPtr->setTransform(transformCar);
        lanePtr->AddCar(carPtr);
        lanes.push_back(lanePtr);
    }

}

void BasicGameApp::createCarTextures(TextureVec& textures) const
{
    textures.clear();
    textures.push_back(std::move(TextureData("../../resources/textures/car.png")));
    textures.push_back(std::move(TextureData("../../resources/textures/car1.png")));
    textures.push_back(std::move(TextureData("../../resources/textures/car2.png")));
    textures.push_back(std::move(TextureData("../../resources/textures/car3.png")));
    textures.push_back(std::move(TextureData("../../resources/textures/car4.png")));
    textures.push_back(std::move(TextureData("../../resources/textures/car5.png")));
    textures.push_back(std::move(TextureData("../../resources/textures/car6.png")));
    textures.push_back(std::move(TextureData("../../resources/textures/car7.png")));
    textures.push_back(std::move(TextureData("../../resources/textures/car8.png")));
}

void BasicGameApp::renderLoop()
{
    ShaderProgram shader("../../resources/shader/v3_shader.vs","../../resources/shader/v3_shader.fs");
    
    TextureData texture_lion("../../resources/textures/player_lion.png");
    TextureData texture_pinguin("../../resources/textures/penguin.png");
    TextureData texture_background("../../resources/textures/green_grass.jpg");
    TextureData texture_road1("../../resources/textures/road4.png");
    TextureData texture_road3("../../resources/textures/road5.png");
    TextureData texture_car("../../resources/textures/car3.png");
    TextureData texture_oups("../../resources/textures/oups.png");
    
 
    float bmin_x =  -1.0f , bmax_x = 1.0f;
    float bmin_y = -1.0f,  bmax_y = 1.0f;
    
    std::vector<float>  vertices_background = {
        // positions           // texture coords
        bmax_x,  bmax_y,  0.0f,   1.0f, 1.0f,
        bmax_x,  bmin_y,  0.0f,   1.0f, 0.0f,
        bmin_x,  bmin_y,  0.0f,   0.0f, 0.0f,
        bmin_x,  bmax_y,  0.0f,   0.0f, 1.0f,
    };
    
    
    float max_y = 0.20, min_y = 0.0f;
    float max_x = 1.0, min_x = -1.0f;
    std::vector<float> vertices_street = {
        // positions           // texture coords
        max_x,   max_y, 0.0f,      1.0f, 1.0f, // top right
        max_x,   min_y, 0.0f,      1.0f, 0.0f, // bottom right
        min_x,  min_y, 0.0f,      0.0f, 0.0f, // bottom left
        min_x,  max_y, 0.0f,      0.0f, 1.0f  // top left
    };

    float start_x = 0.f, start_y = 0.f;
    float car_lenght = 0.4f, car_height = 0.2f;
    float cmin_x =  start_x - car_lenght/2 , cmax_x = start_x + car_lenght/2;
    float cmin_y = start_y, cmax_y = start_y + car_height;
    
    std::vector<float>  vertices_car = {
        // positions           // texture coords
        cmin_x,  cmax_y,  0.0f,   0.0f,  0.0f,
        cmin_x,  cmin_y,  0.0f,   0.0f,  1.0f,
        cmax_x,  cmin_y,  0.0f,   1.0f,  1.0f,
        
        cmin_x,  cmax_y,  0.0f,   0.0f,  0.0f,
        cmax_x,  cmin_y,  0.0f,   1.0f,  1.0f,
        cmax_x,  cmax_y,  0.0f,   1.0f,  0.0f
    };

    // player
    float player_width = 0.30f,  player_height = 0.30f;
    float player_start_x = 0.0f, player_start_y = 0.0f;
    float player_min_x = -(player_start_x +player_width/2) , player_max_x = (player_start_x +player_width/2);
    float player_min_y = -(player_start_y +player_height/2), player_max_y = (player_start_y +player_height/2);
    
    std::vector<float>  vertices_player = {
        // positions                  // texture coords
        player_max_x,  player_max_y,  0.0f,   1.0f, 0.0f, // top right
        player_max_x,  player_min_y,  0.0f,   1.0f, 1.0f , // bottom right
        player_min_x,  player_min_y,  0.0f,   0.0f, 1.0f, // bottom left
        player_min_x,  player_max_y,  0.0f,   0.0f, 0.0f   // top left
    };
    glm::mat4 player_init_pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f , -1.0f + player_max_x, 0.0f));
    
    
    VertexLayout vertexLayoutStreet;
    creteVertexLayout(vertexLayoutStreet);
    
    VertexBuffer bufferStreet;
    const std::size_t stride = 5;
    bufferStreet.create(&vertices_street[0], &vertexLayoutStreet, vertices_street.size() * sizeof(float), stride);
    
    std::vector<unsigned int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    IndexBuffer indexBuff;
    indexBuff.create(&indices[0], indices.size() * sizeof(unsigned int));
    indexBuff.bind();
    
    VertexLayout layout;
    creteVertexLayout(layout);
    
    //car vao
    VertexBuffer bufferCar;
    bufferCar.create(&vertices_car[0], &layout, vertices_car.size() * sizeof(float), stride);
    
    //player vao
    VertexBuffer playerVao;
    playerVao.create(&vertices_player[0], &layout, vertices_player.size() * sizeof(float), stride);
    
    //oups vao
    VertexBuffer oupsVao;
    oupsVao.create(&vertices_player[0], &layout, vertices_player.size() * sizeof(float), stride);
    glm::mat4 oups_init_pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.f , 0.0f, 0.0f));
    
    //background vao
    VertexBuffer backgroundVao;
    backgroundVao.create(&vertices_background[0], &layout, vertices_background.size() * sizeof(float), stride);
    
    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -3.f));
    glm::mat4 player_model = player_init_pos;
    
    std::array<int, 10> enableDrawings{};
    
    
    TextureVec carTextures;
    createCarTextures(carTextures);
    
    LanePtrVec lanes;
    createLanes(lanes, carTextures.size() );

    auto CompareFloat =[&](float a, float b)
    {
       float epsilon = 0.09f;
       float diff = a - b;
       return (diff < epsilon) && (-diff < epsilon);
    };
    
    auto IsPlayerOnLane =[&](float player_y, float lane_y)
    {
        std::cout << CompareFloat(player_y, lane_y) << " player_y = " << player_y << " lane_y = " << lane_y << std::endl;

        return CompareFloat(player_y, lane_y);
    };
    
    auto isPlayerOnCarPosition =[&](float player_x, float car_x)
    {
       // std::cout << " isPlayerOnCarPosition player = " << player_x << " car = " << car_x << std::endl;
        return CompareFloat(player_x, car_x);
    };

    bool isLion = false;
    
    while(!glfwWindowShouldClose(m_window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        m_CameraUtil.deltaTime = currentFrame - m_CameraUtil.lastFrame;
        m_CameraUtil.lastFrame = currentFrame;
        
        processInput(m_window, player_model, enableDrawings);
        
        // clear window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader.bind();
    
        // draw background
        {
            shader.set_uniform("transform", glm::mat4(1.f));
            backgroundVao.bind();
            indexBuff.bind();
            texture_background.bind(0);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        
        
        bool oupsBool = false;
        
        // draw streets and cars
        for(auto itr : lanes)
        {
            bufferStreet.bind();
            indexBuff.bind();
            
            auto lanePos = itr->getLanePos();
            
            bool checkPositionOnLane = false;
            if(IsPlayerOnLane(player_model[3].y - player_height/2, lanePos[3].y))
            {
                checkPositionOnLane = true;
            }

            shader.set_uniform("transform", lanePos);
            texture_road3.bind(0);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            
            bufferCar.bind();
        
            auto cars = itr->getCars();
            for(auto itr : cars)
            {
                auto textureIndex = itr->getTextureIndex();
                carTextures[textureIndex].bind(0);
                
                const auto & carPosition = itr->Move();
                shader.set_uniform("transform", carPosition);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                
                // car reached the end -> set car to origin
                if( carPosition[3].x > 1.f + car_lenght)
                {
                    auto init_y = carPosition[3].y;
                    auto init_x = -1.f;
                    
                    auto initPos = glm::translate(glm::mat4(1.0f), glm::vec3(init_x, init_y, 0.f));;
                    itr->setTransform(initPos);
                    
                    // change texture
                    int texture = rand()%(carTextures.size()-1);
                    itr->setTextureIndex(texture);
                }
                
                if(checkPositionOnLane)
                {
                     if(isPlayerOnCarPosition(player_model[3].x, carPosition[3].x + car_lenght/2))
                    {
                        oupsBool = true;
                    }
                }
                
//                std::cout << "Car = " << carPosition[3].x << ", " << carPosition[3].y << std::endl;
//                std::cout << "Player = " << player_model[3].x << ", " << player_model[3].y << std::endl;
                
//                if( carPosition[3].x == player_model[3].x && carPosition[3].y == player_model[3].y )
//                {
//                    oupsBool = true;
//                }
                
//                if( carPosition[3].x >= -0.28f && carPosition[3].y  <= 0.1f )
//                {
//                    oupsBool = true;
//                }
            }
            
        }
        
        // draw player
        {
            playerVao.bind();
            indexBuff.bind();
           // texture_lion.bind(0);
            
            if(isLion)
            {
                texture_lion.bind(0);
            }
            else
            {
                texture_pinguin.bind(0);
            }

            // translate player to initial position
            if( player_model[3].y >= 1.f )
            {
                player_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f , -1.0f + player_max_x, 0.0f));
                isLion  = !isLion;
            }
            if( player_model[3].y <= -1.f )
            {
                player_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f , 1.0f - player_max_x, 0.0f));
            }
            
            if( player_model[3].x >= 1.f )
            {
                player_model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f + player_max_x, 0.0f, 0.0f));
            }
            
            if( player_model[3].x <= -1.f )
            {
                player_model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f - player_max_x, 0.0f , 0.0f));
            }
            
            
            shader.set_uniform("transform", player_model);
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
                
        
        // oups
        if(oupsBool)
        {
            oupsVao.bind();
            indexBuff.bind();
            texture_oups.bind(0);
            
            shader.set_uniform("transform", oups_init_pos);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            oupsBool = false;
        }


        
        
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    
    //buffer.clear();
    // indexBuff.clear();
    glfwTerminate();
}

void BasicGameApp::creteVertexLayout(VertexLayout& vertexLayout)
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

void BasicGameApp::processInput(GLFWwindow *window, glm::mat4& transformMat, std::array<int, 10>& enableDrawings)
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

void BasicGameApp::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

void BasicGameApp::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    m_camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


