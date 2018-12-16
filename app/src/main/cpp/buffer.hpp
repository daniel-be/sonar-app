//
// Created by Daniel on 06.12.2018.
//

#ifndef SONAR_APP_BUFFER_HPP
#define SONAR_APP_BUFFER_HPP

#include <vector>

#include "vertex_attribute.hpp"

template <typename T>
class Buffer
{
public:
    Buffer(const T* data, unsigned int gl_target, unsigned long int size, unsigned int usage);
    void bind() const;
    void unbind() const;
    void add_vertex_attribute(Vertex_attribute<T> vertex_attribute);
    void enable_vertex_attributes() const;
    ~Buffer();

private:
    unsigned int gl_id;
    unsigned int gl_target;
    std::vector<Vertex_attribute<T>> vertex_attributes;
};

#endif //SONAR_APP_BUFFER_HPP
