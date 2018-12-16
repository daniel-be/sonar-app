//
// Created by Daniel on 06.12.2018.
//
#include <GLES2/gl2.h>

#include "buffer.hpp"
#include "utility.hpp"
#include "log.h"

template <typename T>
Buffer<T>::Buffer(const T* data, unsigned int gl_target, unsigned long int size, unsigned int usage)
        : gl_id(0), gl_target(gl_target)
{
    GL_CALL(glGenBuffers(1, &(this->gl_id)));
    GL_CALL(glBindBuffer(this->gl_target, this->gl_id));
    GL_CALL(glBufferData(this->gl_target, sizeof(T) * size, data, usage));
}

template <typename T>
void Buffer<T>::bind() const
{
    GL_CALL(glBindBuffer(this->gl_target, this->gl_id));
}

template <typename T>
void Buffer<T>::unbind() const
{
    GL_CALL(glBindBuffer(this->gl_target, 0));
}

template <typename T>
void Buffer<T>::add_vertex_attribute(Vertex_attribute<T> vertex_attribute)
{
    this->vertex_attributes.push_back(vertex_attribute);
}

template <typename T>
void Buffer<T>::enable_vertex_attributes() const
{
    for (auto vertex_attribute : this->vertex_attributes)
    {
        vertex_attribute.enable();
    }
}

template <typename T>
Buffer<T>::~Buffer()
{
    GL_CALL(glDeleteBuffers(1, &(this->gl_id)));
}
