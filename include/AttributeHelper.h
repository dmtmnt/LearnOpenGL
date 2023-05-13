#pragma once

//A simple enum listing all the attributes that your engine "supports"

enum AttributeType
{
    Position = 0,
    Color,
    Texture,
    //add more you need

    Count
};

struct AttributeHelper
{
    static const char* get_name(AttributeType);
    static AttributeType get_type(const char* attribute_name);
};
