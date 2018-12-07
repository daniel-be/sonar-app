//
// Created by Daniel on 30.11.2018.
//

#ifndef SONAR_APP_ENGINE_HPP
#define SONAR_APP_ENGINE_HPP

#include <string>

#include "shader.hpp"

class Engine
{
public:
    Engine();
    void init();
    void change(unsigned int width, unsigned int height);
    void draw_frame(int distance, int angle);
    ~Engine();

private:
    static const std::string vertex_shader_source;
    static const std::string fragment_shader_source;
    Shader shader;
};

#endif //SONAR_APP_ENGINE_HPP
