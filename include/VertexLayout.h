#pragma once

#include "AttributeHelper.h"
#include <cstddef>
#include <vector>
//#include "uniformHelper.h"

//Vertex layout helps you describe the structure of a vertex
//A vertex can have multiple attribues in any given order

struct VertexAttribute
{
    AttributeType type;
    std::size_t location = 0;
    std::size_t offset = 0;
    std::size_t number_of_floats = 0;
};

class VertexLayout
{
public:
    VertexLayout() = default;

    std::size_t size() const;
    //^ returns the size in bytes of one attribute
 
    VertexAttribute CreateVertexAttribute(AttributeType type, std::size_t location, std::size_t offset, std::size_t number_of_floats);
                                       
    void AddVertexAttribute(const VertexAttribute& attr);
    //adds the coresponding attributes that describes the vertex data.

    std::size_t getAttributeCount() const;

    VertexAttribute getAttribute(const std::size_t attribute_index) const;
private:
    std::vector<VertexAttribute> m_vertexAttributeVec;

};

//tn::vector<tn::gfx::InputComponent>     vertexAttrsLayout;
//tn::vector<char>                        vertexBuffer;
