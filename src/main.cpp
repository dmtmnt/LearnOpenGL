//#include <iostream>
#include <Application.h>
#include <BasicLightingApp.h>
#include <Basic3DApp.h>
#include <Basic2DApp.h>
#include <BasicGameApp.h>
#include <unordered_map>
#include <map>
#include <vector>
#include <stack>
#include <math.h>
#include <deque>
#include <random>
#include <cstdio>


int main()
{
    
    // settings
    const std::size_t width = 1200, height = 1000;
    const char * window_name = "LearnOpenGL";

    
//    BasicLightingApp demoLight("BasicLighingDemo", width, height);
//    demoLight.renderLoopBasicLighting();
//
//    Basic3DApp demo3D("Basic3DDemo", width, height);
//    demo3D.renderLoop();
//
    BasicGameApp demoGame("CrossStreetGame", width, height);
    demoGame.renderLoop();
    
//    BasicLightingApp demoLight("BasicLighingDemo", width, height);
//    demoLight.renderLoop();
//
//    Basic2DApp demo2D("ShadersAndTransforms", width, height);
//    demo2D.renderLoop();
    
//    Application app(window_name, width, height);
//    app.renderLoop();

}

// missing

