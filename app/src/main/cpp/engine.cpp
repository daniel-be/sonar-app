//
// Created by Daniel on 30.11.2018.
//
#include <string>
#include <cmath>

#include <GLES2/gl2.h>

#include "log.h"
#include "engine.hpp"
#include "utility.hpp"
#include "vertex_attribute.hpp"
#include "vertex.hpp"

Engine::Engine() : shader(nullptr), vertex_buffer(nullptr) { }

void Engine::init()
{
    Utility::print_gl_version();
    this->shader = std::shared_ptr<Shader>{ new Shader{ this->vertex_shader_src, this->fragment_shader_src } };
    this->generate_triangle_vertices();
    int position_handle = this->shader->get_attribute_location("vPosition");
    int color_handle = this->shader->get_attribute_location("vColor");
    int dist_handle = this->shader->get_attribute_location("vDist");
    this->vertex_buffer->bind();
    Vertex_attribute<Vertex> vertex_attribute_pos{ static_cast<unsigned int>(position_handle), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position) };
    Vertex_attribute<Vertex> vertex_attribute_color{ static_cast<unsigned int>(color_handle), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color) };
    Vertex_attribute<Vertex> vertex_attribute_dist{ static_cast<unsigned int>(dist_handle), 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, distance) };
    this->vertex_buffer->add_vertex_attribute(vertex_attribute_pos);
    this->vertex_buffer->add_vertex_attribute(vertex_attribute_color);
    this->vertex_buffer->unbind();
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Engine::change(unsigned int width, unsigned int height)
{
    GL_CALL(glViewport(0, 0, width, height));
    this->set_proj_matrix(width, height);
}

void Engine::draw_frame(float dist, int angle)
{
    this->reset_time_point(angle);
    if (dist > 0.0f) this->set_distance(angle, dist);
    this->fade_triangles();
    this->opengl_draw();
}

void Engine::set_proj_matrix(unsigned int width, unsigned int height) const
{
    float ratio = static_cast<float>(height) / static_cast<float>(width);
    float proj_matrix[16];
    proj_matrix[0] = ratio;
    proj_matrix[1] = 0;
    proj_matrix[2] = 0;
    proj_matrix[3] = 0;

    proj_matrix[4] = 0;
    proj_matrix[5] = 1;
    proj_matrix[6] = 0;
    proj_matrix[7] = 0;

    proj_matrix[8] = 0;
    proj_matrix[9] = 0;
    proj_matrix[10] = 1;
    proj_matrix[11] = 0;

    proj_matrix[12] = 0;
    proj_matrix[13] = 0;
    proj_matrix[14] = 0;
    proj_matrix[15] = 1;
    int proj_matrix_handle = this->shader->get_uniform_location("vOrthoMatrix");
    this->shader->bind();
    this->shader->set_uniform_matrix4fv_value(proj_matrix_handle, proj_matrix);
}

void Engine::generate_triangle_vertices()
{
    std::array<float, 4> circle_center = { Engine::CIRCLE_CENTER_X, Engine::CIRCLE_CENTER_Y, 0.0f, 1.0f };
    std::array<float, 4> initial_color =  { 0.0f, 1.0f, 0.0f, 1.0f };
    float intital_dist = 0.0f;

    Vertex v0{
        .position = circle_center,
        .color = initial_color,
        .distance = intital_dist
    };
    Vertex v1{
        .position = { static_cast<float>(Engine::RADIUS * std::cos(359.5f * M_PI / 180.0f)),
                      static_cast<float>(Engine::CIRCLE_CENTER_Y + Engine::RADIUS * std::sin(359.5f * M_PI / 180.0f)),
                      0.0f,
                      1.0f },
        .color = initial_color,
        .distance = intital_dist
    };
    Vertex v2{
        .position = { static_cast<float>(Engine::RADIUS * std::cos(0.5f * M_PI / 180.0f)),
                      static_cast<float>(Engine::CIRCLE_CENTER_Y + Engine::RADIUS * std::sin(0.5f * M_PI / 180.0f)),
                      0.0f,
                      1.0f },
        .color = initial_color,
        .distance = intital_dist
    };

    std::vector<Triangle> triangles = { Triangle{ v0, v1, v2 } };
    triangles.reserve(Engine::DEGREES);

    for (int i = 1; i < Engine::DEGREES; i++) {
        Vertex v1{
                .position = { static_cast<float>(Engine::RADIUS * std::cos((i - 0.5f) * M_PI / 180.0f)),
                              static_cast<float>(Engine::CIRCLE_CENTER_Y + Engine::RADIUS * std::sin((i - 0.5f) * M_PI / 180.0f)),
                              0.0f,
                              1.0f },
                .color = initial_color,
                .distance = intital_dist
        };
        Vertex v2{
                .position = { static_cast<float>(Engine::RADIUS * std::cos((i + 0.5f) * M_PI / 180.0f)),
                              static_cast<float>(Engine::CIRCLE_CENTER_Y + Engine::RADIUS * std::sin((i + 0.5f) * M_PI / 180.0f)),
                              0.0f,
                              1.0f },
                .color = initial_color,
                .distance = intital_dist
        };

        triangles.push_back(Triangle{ v0, v1, v2 });
    }

    this->vertex_buffer = std::shared_ptr<Buffer<Triangle, Vertex>>{
            new Buffer<Triangle, Vertex>{ static_cast<const Triangle*>(triangles.data()),
                                          GL_ARRAY_BUFFER, Engine::DEGREES,
                                          GL_DYNAMIC_DRAW }
    };
}

void Engine::opengl_draw() const
{
    this->shader->bind();
    this->vertex_buffer->bind();
    this->vertex_buffer->enable_vertex_attributes();
    GL_CALL(glClearColor(0.0, 0.0, 0.0, 1.0));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, Engine::DEGREES * 3));
    this->vertex_buffer->disable_vertex_attributes();
    this->vertex_buffer->unbind();
    this->shader->unbind();
}

void Engine::fade_triangles() const
{
    float new_alpha = 0.0f;
    this->vertex_buffer->bind();
    for (int i = 0; i < Engine::DEGREES; i++) {
        std::chrono::milliseconds duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->triangle_start_points[i]);
        float alpha_val = (-1.0f / 2000.0f) * duration.count() + 1.0f;
        new_alpha = alpha_val >= 0 ? alpha_val : 0;
        for (int j = 0; j < 3; j++) {
            this->vertex_buffer->replace_data(i * sizeof(Triangle) + j * sizeof(Vertex) + offsetof(Vertex, color) + sizeof(float) * 3, sizeof(new_alpha), &new_alpha);
        }
    }
    this->vertex_buffer->unbind();
}

void Engine::reset_time_point(int index)
{
    this->triangle_start_points[index] = std::chrono::steady_clock::now();
}

void Engine::set_distance(int angle, float dist) const
{
    float swap_color[] = { 1.0, 0.0 };
    this->vertex_buffer->bind();
    for (int i = 0; i < 3; i++) {
        this->vertex_buffer->replace_data(angle * sizeof(Triangle) + i * sizeof(Vertex) + offsetof(Vertex, color), sizeof(swap_color), &swap_color);
        this->vertex_buffer->replace_data(angle * sizeof(Triangle) + i * sizeof(Vertex) + offsetof(Vertex, distance), sizeof(dist), &dist);
    }
    this->vertex_buffer->unbind();
}

Engine::~Engine() { }
