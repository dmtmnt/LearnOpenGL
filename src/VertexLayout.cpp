#include "VertexLayout.h"
#include "AttributeHelper.h"
#include <iostream>

std::size_t VertexLayout::size() const
{
    // todo
    return 0; // (size_t) sizeof(float) * number_of_floats;
    //^ returns the size in bytes of one attribute
}
    
VertexAttribute
VertexLayout::CreateVertexAttribute(AttributeType type, std::size_t location, std::size_t offset, std::size_t number_of_floats)
{
    VertexAttribute attr;;
    attr.type = type;
    attr.location = location;
    attr.offset = offset;
    attr.number_of_floats = number_of_floats;
    return attr;
}

void
VertexLayout::AddVertexAttribute(const VertexAttribute& attr)
{
    m_vertexAttributeVec.push_back(attr);
}
    
std::size_t
VertexLayout::getAttributeCount() const
{
    return m_vertexAttributeVec.size();
}

VertexAttribute
VertexLayout::getAttribute(const std::size_t attribute_index) const
{
    return m_vertexAttributeVec[attribute_index];
}


