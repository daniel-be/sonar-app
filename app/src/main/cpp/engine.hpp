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
            "uniform mat4 vOrthoMatrix;\n"
            "attribute vec4 vPosition;\n"
            "attribute vec4 vColor;\n"
            "attribute float vDist;\n"
            "varying vec4 fColor;\n"
            "varying float fDist;\n"
            "void main() {\n"
            "fColor = vColor;\n"
            "fDist = vDist;\n"
            "gl_Position = vOrthoMatrix * vPosition;\n"
            "}\n";

    const std::string fragment_shader_src =
            "precision mediump float;\n"
            "varying vec4 fColor;\n"
            "varying float fDist;\n"
            "void main() {\n"
            "float dist = fDist;\n"
            "gl_FragColor = fColor + (fDist-fDist);\n"
            "}\n";

    std::shared_ptr<Buffer<Triangle, Vertex>> vertex_buffer;
    std::shared_ptr<Shader> shader;

    void set_proj_matrix(unsigned int width, unsigned int height) const;
    void generate_triangle_vertices();
    void opengl_draw() const;
    void fade_triangles() const;
    void reset_time_point(int index);
    void set_distance(int angle, float dist) const;
};

#endif //SONAR_APP_ENGINE_HPP
