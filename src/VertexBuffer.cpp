#include "VertexBuffer.h"
#include "VertexLayout.h"
#include <glew.h>
#include <glfw3.h>
#include <iostream>

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindVertexArray(m_vao);
    
}

void VertexBuffer::unbind() const
{
    glBindVertexArray(0); //unbinding VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding VBO
}

void VertexBuffer::clear() 
{
    glDeleteVertexArrays(1, &m_vbo);
    glDeleteBuffers(1, &m_vao);
}

void VertexBuffer::create(const void* buffer, const VertexLayout* layout, const std::size_t buffer_size, const std::size_t stride)
{
    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);
    bind();
   
    glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer, GL_STATIC_DRAW);
    
    for (size_t i = 0; i < layout->getAttributeCount(); ++i)
    {
        const VertexAttribute& attr = layout->getAttribute(i);
        
        glVertexAttribPointer(i, attr.number_of_floats, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)( attr.offset* sizeof(float)));
        glEnableVertexAttribArray(i);
 
        
        std::cout << "Loc = " << i << " number_of_floats = " << attr.number_of_floats
        << " stride = " << stride <<  " attr.offset = " << attr.offset* sizeof(float) << std::endl;
        
       }
    
    unbind();
}
    

