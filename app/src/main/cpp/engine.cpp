//
// Created by Daniel on 30.11.2018.
//
#include <string>

#include <GLES2/gl2.h>

#include "log.h"
#include "engine.hpp"
#include "shader.hpp"
#include "utility.hpp"
#include "buffer.hpp"
#include "buffer.cpp"
#include "vertex_attribute.cpp"

const std::string Engine::vertex_shader_source =
        "attribute vec4 vPosition;\n"
        "void main() {\n"
        "  gl_Position = vPosition;\n"
        "}\n";

const std::string Engine::fragment_shader_source =
        "precision mediump float;\n"
        "void main() {\n"
        "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
        "}\n";

const float Engine::vertices[6] = {
            0.0f, 0.5f,
            -0.5f, -0.5f,
            0.5f, -0.5f
        } ;

Engine::Engine()
        : shader(Shader { Engine::vertex_shader_source, Engine::fragment_shader_source }),
          vertex_buffer(Buffer<float> { &vertices[0], GL_ARRAY_BUFFER, sizeof(float) * 6, GL_STATIC_DRAW }) { }

void Engine::init()
{
    Utility::print_gl_version();

    int position_handle = this->shader.get_attribute_location("vPosition");
    LOG_I("Engine.init", "position_handle: %d", position_handle);
    this->vertex_buffer.bind();
    Vertex_attribute<float> vertex_attribute {static_cast<unsigned>(position_handle), 2, GL_FLOAT, GL_FALSE, 0 };
    this->vertex_buffer.add_vertex_attribute(vertex_attribute);
}

void Engine::change(unsigned int width, unsigned int height)
{
    GL_CALL(glViewport(0, 0, width, height));
}

void Engine::draw_frame(int distance, int angle)
{
    GL_CALL(glClearColor(0.01, 0.01, 0.01, 1.0));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    this->shader.bind();
    this->vertex_buffer.enable_vertex_attributes();
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
}

Engine::~Engine() { }
