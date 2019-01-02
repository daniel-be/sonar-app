//
// Created by daniel on 12/28/2018.
//

#ifndef SONAR_APP_VERTEX_HPP
#define SONAR_APP_VERTEX_HPP

#include <array>

struct Vertex
{
    std::array<float, 4> position;
    std::array<float, 4> color;
    float distance;
};

#endif //SONAR_APP_VERTEX_HPP
