#include <IndexBuffer.h>
#include <glew.h>
#include <glfw3.h>

IndexBuffer::IndexBuffer(){}
IndexBuffer::~IndexBuffer(){}


void IndexBuffer::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void IndexBuffer::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void IndexBuffer::create(const uint32_t* data, size_t size)
{
    glGenBuffers(1, &m_ebo);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void IndexBuffer::clear()
{
    glDeleteBuffers(1, &m_ebo);
}

