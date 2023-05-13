#pragma once
#include <cstddef>
#include <string>
#include <glew.h>
#include <glfw3.h>
#include <vector>

class TextureData
{
public:
    TextureData(const std::string& file_path);
    ~TextureData();
    
    void bind(int slot);
    
private:
    bool load(const std::string& file_path);
private:
    unsigned int m_texture;
};

typedef std::vector<TextureData> TextureVec;
