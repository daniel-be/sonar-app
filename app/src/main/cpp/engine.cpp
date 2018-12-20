//
// Created by Daniel on 30.11.2018.
//
#include <string>
#include <cmath>

#include <GLES2/gl2.h>

#include "log.h"
#include "engine.hpp"
#include "utility.hpp"
#include "buffer.cpp"
#include "vertex_attribute.hpp"
#include "vertex_attribute.cpp"

Engine::Engine() : shader(nullptr), vertex_buffer(nullptr)
{
    this->shader = std::shared_ptr<Shader>{ new Shader{ this->vertex_shader_src, this->fragment_shader_src } };
    float vertices[363];
    vertices[0] = 0.0f;
    vertices[1] = -0.5f;
    for (int i = 0; i < 181; i++) {
        vertices[i * 2] = 1.0f * std::cos(i * M_PI / 180);
        vertices[i * 2 + 1] = -0.5f + 1.0f * std::sin(i * M_PI / 180);
    }
    this->vertex_buffer = std::shared_ptr<Buffer<float>>{ new Buffer<float>{ vertices, GL_ARRAY_BUFFER, sizeof(float) * 363, GL_STATIC_DRAW } };
}

void Engine::init()
{
    Utility::print_gl_version();
    int position_handle = this->shader->get_attribute_location("vPosition");
    LOG_I("Engine.init", "position_handle: %d", position_handle);
    this->vertex_buffer->bind();
    Vertex_attribute<float> vertex_attribute {static_cast<unsigned int>(position_handle), 2, GL_FLOAT, GL_FALSE, 0 };
    this->vertex_buffer->add_vertex_attribute(vertex_attribute);
}

void Engine::change(unsigned int width, unsigned int height)
{
    GL_CALL(glViewport(0, 0, width, height));
}

void Engine::draw_frame(int distance, int angle)
{
    GL_CALL(glClearColor(0.01, 0.01, 0.01, 1.0));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    this->shader->bind();
    this->vertex_buffer->enable_vertex_attributes();
    GL_CALL(glDrawArrays(GL_TRIANGLE_FAN, 0, 182));
}

Engine::~Engine()
{
    delete[] this->vertices;
}
