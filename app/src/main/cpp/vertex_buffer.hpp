//
// Created by Daniel on 04.12.2018.
//

#ifndef SONAR_APP_VERTEXBUFFER_HPP
#define SONAR_APP_VERTEXBUFFER_HPP

#include "vertex.hpp"

class Vertex_buffer
{
public:
    Vertex_buffer(Vertex* data, signed long int size);
    void bind() const;
    void unbind() const;
    ~Vertex_buffer();

private:
    unsigned int gl_id;
};

#endif //SONAR_APP_VERTEXBUFFER_HPP
