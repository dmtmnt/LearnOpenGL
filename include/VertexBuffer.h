#pragma once
#include <iostream>

class VertexLayout;

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
    void create(const void* buffer, const VertexLayout* layout, const std::size_t buffer_size, const std::size_t stride);
    void clear();
private:
    uint32_t m_vbo = 0;
    uint32_t m_vao = 0;
};

