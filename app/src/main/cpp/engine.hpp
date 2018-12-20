//
// Created by Daniel on 30.11.2018.
//

#ifndef SONAR_APP_ENGINE_HPP
#define SONAR_APP_ENGINE_HPP

#include <string>
#include <memory>

#include "shader.hpp"
#include "buffer.hpp"

class Engine
{
public:
    Engine();
    void init();
    void change(unsigned int width, unsigned int height);
    void draw_frame(int distance, int angle);
    ~Engine();

private:
    float* vertices;
    const std::string vertex_shader_src =
            "attribute vec4 vPosition;\n"
            "void main() {\n"
            "gl_Position = vPosition;\n"
            "}\n";

    const std::string fragment_shader_src =
            "precision mediump float;\n"
            "void main() {\n"
            "gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
            "}\n";

    std::shared_ptr<Buffer<float>> vertex_buffer;
    std::shared_ptr<Shader> shader;
};

#endif //SONAR_APP_ENGINE_HPP
