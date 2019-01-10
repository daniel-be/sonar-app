//
// Created by Daniel on 30.11.2018.
//

#ifndef SONAR_APP_ENGINE_HPP
#define SONAR_APP_ENGINE_HPP

#include <string>
#include <memory>
#include <chrono>

#include "shader.hpp"
#include "buffer.hpp"
#include "triangle.hpp"

class Engine
{
public:
    Engine();
    void init();
    void change(unsigned int width, unsigned int height);
    void draw_frame(float dist, int angle);
    ~Engine();

private:
    static constexpr int DEGREES = 181;
    static constexpr float RADIUS = 2.0f;
    static constexpr float CIRCLE_CENTER_X = 0.0f;
    static constexpr float CIRCLE_CENTER_Y = -1.0f;
    static constexpr float MAX_DIST = 40.0f;
    std::chrono::steady_clock::time_point triangle_start_points[Engine::DEGREES];
    const std::string vertex_shader_src =
            "uniform mat4 ortho_matrix;\n"
            "attribute vec4 a_position;\n"
            "attribute vec4 a_color;\n"
            "attribute float a_dist;\n"
            "varying vec4 v_color;\n"
            "varying float v_dist;\n"
            "void main() {\n"
            "v_color = a_color;\n"
            "v_dist = a_dist;\n"
            "gl_Position = ortho_matrix * a_position;\n"
            "}\n";

    const std::string fragment_shader_src =
            "precision mediump float;\n"
            "uniform float width;\n"
            "uniform float height;\n"
            "varying vec4 v_color;\n"
            "varying float v_dist;\n"
            "void main() {\n"
            "float dist = v_dist;\n"
            "float rad = height * (dist / 50.0);\n"
            "vec4 width_center = vec4(width / 2.0, 0.0, 0.0, 1.0);\n"
            "if (rad > distance(width_center, gl_FragCoord)) {\n"
            "vec4 color = v_color;\n"
            "color.r = 0.0;\n"
            "color.g = 1.0;\n"
            "gl_FragColor = color;\n"
            "}\n"
            "else {\n"
            "gl_FragColor = v_color;\n"
            "}\n"
            "}\n";

    std::shared_ptr<Buffer<Triangle, Vertex>> vertex_buffer;
    std::shared_ptr<Shader> shader;

    void set_proj_matrix(float l, float r, float b, float t, float n, float f) const;
    void generate_triangle_vertices();
    void opengl_draw() const;
    void fade_triangles() const;
    void reset_time_point(int index);
    void set_distance(int angle, float dist) const;
};

#endif //SONAR_APP_ENGINE_HPP
