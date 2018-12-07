//
// Created by Daniel on 30.11.2018.
//
#include <string>

#include <GLES2/gl2.h>

#include "log.h"
#include "engine.hpp"
#include "shader.hpp"
#include "utility.hpp"

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

Engine::Engine()
        : shader(Shader { Engine::vertex_shader_source, Engine::fragment_shader_source }) { }

void Engine::init()
{
    Utility::print_gl_version();
}

void Engine::change(unsigned int width, unsigned int height)
{
    GL_CALL(glViewport(0, 0, width, height));
}

void Engine::draw_frame(int distance, int angle)
{
    GL_CALL(glClearColor(1.0, 1.0, 1.0, 1.0));
}

Engine::~Engine() { }
