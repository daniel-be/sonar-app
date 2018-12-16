//
// Created by daniel on 12/7/2018.
//
#include <GLES2/gl2.h>

#include "vertex_attribute.hpp"
#include "log.h"

template <typename T>
Vertex_attribute<T>::Vertex_attribute(unsigned int index, int size, int type,
                                      int normalized, const void* pointer)
    : index(index), size(size), type(type), normalized(normalized), pointer(pointer)
{
    GL_CALL(glVertexAttribPointer(this->index, this->size, this->type, this->normalized,
                                  this->size * sizeof(T), this->pointer));
}

template <typename T>
void Vertex_attribute<T>::enable() const
{
    GL_CALL(glEnableVertexAttribArray(this->index));
}

template <typename T>
void Vertex_attribute<T>::disable() const
{
    GL_CALL(glDisableVertexAttribArray(this->index));
}

template <typename T>
Vertex_attribute<T>::~Vertex_attribute() {}