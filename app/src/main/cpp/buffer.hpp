//
// Created by Daniel on 06.12.2018.
//

#ifndef SONAR_APP_BUFFER_HPP
#define SONAR_APP_BUFFER_HPP

#include <GLES2/gl2.h>

#include <vector>

#include "vertex_attribute.hpp"

template <typename T1, typename T2>
class Buffer
{
public:
    Buffer();
    Buffer(const T1* data, unsigned int gl_target, unsigned long int size, unsigned int usage);
    void bind() const;
    void unbind() const;
    void replace_data(signed long int offset, signed long int size, const void* data) const;
    void add_vertex_attribute(Vertex_attribute<T2> vertex_attribute);
    void enable_vertex_attributes() const;
    void disable_vertex_attributes() const;
    ~Buffer();

private:
    unsigned int gl_id;
    unsigned int gl_target;
    std::vector<Vertex_attribute<T2>> vertex_attributes;
};

template <typename T1, typename T2>
Buffer<T1, T2>::Buffer() : gl_id(0), gl_target(0) {}

template <typename T1, typename T2>
Buffer<T1, T2>::Buffer(const T1* data, unsigned int gl_target, unsigned long int size, unsigned int usage)
        : gl_id(0), gl_target(gl_target)
{
    GL_CALL(glGenBuffers(1, &(this->gl_id)));
    GL_CALL(glBindBuffer(this->gl_target, this->gl_id));
    GL_CALL(glBufferData(this->gl_target, sizeof(T1) * size, data, usage));
}

template <typename T1, typename T2>
void Buffer<T1, T2>::bind() const
{
    GL_CALL(glBindBuffer(this->gl_target, this->gl_id));
}

template <typename T1, typename T2>
void Buffer<T1, T2>::unbind() const
{
    GL_CALL(glBindBuffer(this->gl_target, 0));
}

template <typename T1, typename T2>
void Buffer<T1, T2>::replace_data(signed long int offset, signed long int size, const void* data) const
{
    GL_CALL(glBufferSubData(this->gl_target, offset, size, data));
}

template <typename T1, typename T2>
void Buffer<T1, T2>::add_vertex_attribute(Vertex_attribute<T2> vertex_attribute)
{
    this->vertex_attributes.push_back(vertex_attribute);
}

template <typename T1, typename T2>
void Buffer<T1, T2>::enable_vertex_attributes() const
{
    for (auto vertex_attribute : this->vertex_attributes)
    {
        vertex_attribute.enable();
    }
}

template <typename T1, typename T2>
void Buffer<T1, T2>::disable_vertex_attributes() const {
    for (auto vertex_attribute : this->vertex_attributes)
    {
        vertex_attribute.disable();
    }
}

template <typename T1, typename T2>
Buffer<T1, T2>::~Buffer()
{
    GL_CALL(glDeleteBuffers(1, &(this->gl_id)));
}

#endif //SONAR_APP_BUFFER_HPP
