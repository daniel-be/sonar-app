//
// Created by daniel on 12/7/2018.
//

#ifndef SONAR_APP_VERTEX_ATTRIBUTE_HPP
#define SONAR_APP_VERTEX_ATTRIBUTE_HPP

template <typename T>
struct Vertex_attribute
{
    unsigned int index;
    int size;
    int type;
    int normalized;
    const void* pointer;

    Vertex_attribute(unsigned int index, int size, int type, int normalized, const void* pointer);
    void enable() const;
    void disable() const;
    ~Vertex_attribute();
};

#endif //SONAR_APP_VERTEX_ATTRIBUTE_HPP
