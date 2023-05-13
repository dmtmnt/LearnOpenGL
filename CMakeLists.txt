cmake_minimum_required(VERSION 3.21)

project(OpenGLTestCode)

set(CMAKE_CXX_STANDARD 14)
set(TARGET_NAME OpenGLTestCode)

 # Add header file
set(GLEW_H /usr/local/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /usr/local/Cellar/glfw/3.3.8/include/GLFW)
include_directories(${GLEW_H} ${GLFW_H})
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/third_party/glm)

 # Add target link
set(GLEW_LINK /usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.8/lib/libglfw.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

 # Execute compilation command
file(GLOB_RECURSE DEMO_HEADERS include/*.h)
file(GLOB_RECURSE DEMO_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_executable(${TARGET_NAME} ${DEMO_HEADERS} ${DEMO_SOURCES} )

if (APPLE)
    target_link_libraries(${TARGET_NAME} "-framework OpenGL")
    target_link_libraries(${TARGET_NAME} "-framework GLUT")
endif()


