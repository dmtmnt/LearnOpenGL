#pragma once
#include <iostream>


class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer();

    inline std::size_t get_index_count() const { return m_index_count; };

    void bind();
    void unbind();
    void create(const uint32_t* data, const std::size_t index_count);
    void clear();
private:
    std::size_t m_index_count = 0;
    unsigned int m_ebo = 0;
};
