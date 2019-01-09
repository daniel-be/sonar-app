//
// Created by daniel on 12/7/2018.
//

#ifndef SONAR_APP_VERTEX_ATTRIBUTE_HPP
#define SONAR_APP_VERTEX_ATTRIBUTE_HPP

#include <string>

#include <GLES2/gl2.h>

#include "log.h"

template <typename T>
struct Vertex_attribute
{
    unsigned int index;
    int size, type, normalized, stride, offset;

    Vertex_attribute(unsigned int index, int size, int type, int normalized, int stride, int offset);
    void enable() const;
    void disable() const;
    ~Vertex_attribute();
};

template <typename T>
Vertex_attribute<T>::Vertex_attribute(unsigned int index, int size, int type,
                                      int normalized, int stride, int offset)
        : index(index), size(size), type(type), normalized(normalized), stride(stride), offset(offset)
{
    GL_CALL(glVertexAttribPointer(this->index, this->size, this->type, this->normalized, this->stride, (void *)this->offset));
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

#endif //SONAR_APP_VERTEX_ATTRIBUTE_HPP
