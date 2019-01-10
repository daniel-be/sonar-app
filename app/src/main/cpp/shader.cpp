//
// Created by Daniel on 06.12.2018.
//
#include <alloca.h>
#include <GLES2/gl2.h>

#include "log.h"
#include "shader.hpp"

Shader::Shader() : gl_vertex_shader_id(0), gl_fragment_shader_id(0), gl_program_id(0) { }

Shader::Shader(const std::string &vertex_shader_src, const std::string &fragment_shader_src)
{
    this->gl_vertex_shader_id = load_shader(GL_VERTEX_SHADER, vertex_shader_src.c_str());
    this->gl_fragment_shader_id = load_shader(GL_FRAGMENT_SHADER, fragment_shader_src.c_str());
    this->gl_program_id = make_program();
}

void Shader::unbind() const
{
    GL_CALL(glUseProgram(0));
}

void Shader::bind() const
{
    GL_CALL(glUseProgram(this->gl_program_id));
}

int Shader::get_attribute_location(const std::string name) const
{
    GL_CALL(return glGetAttribLocation(this->gl_program_id, name.c_str()));
}

int Shader::get_uniform_location(const std::string name) const
{
    GL_CALL(return glGetUniformLocation(this->gl_program_id, name.c_str()));
}

void Shader::set_uniform_matrix4fv_value(int location, const float* matrix) const
{
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, matrix));
}

void Shader::set_uniform_1f_value(int location, const float value) const
{
    GL_CALL(glUniform1f(location, value));
}

unsigned int Shader::load_shader(unsigned int shader_type, const char* const shader_src)
{
    GL_CALL(unsigned int shader = glCreateShader(shader_type));
    GL_CALL(glShaderSource(shader, 1, &shader_src, NULL));
    GL_CALL(glCompileShader(shader));

    int compiled;
    GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));
    if (compiled == GL_FALSE)
    {
        int log_length;
        GL_CALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length));
        char* info_log = (char*)alloca(sizeof(char) * log_length);
        GL_CALL(glGetShaderInfoLog(shader, log_length, NULL, info_log));
        LOG_E("Shader.load_shader", "Info Log: %s", info_log);
        GL_CALL(glDeleteShader(shader));
        return 0;
    }

    return shader;
}

unsigned int Shader::make_program()
{
    GL_CALL(int program = glCreateProgram());
    GL_CALL(glAttachShader(program, this->gl_vertex_shader_id));
    GL_CALL(glAttachShader(program, this->gl_fragment_shader_id));
    GL_CALL(glLinkProgram(program));

    int linked;
    GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &linked));

    if (linked == GL_FALSE)
    {
        int log_length;
        GL_CALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length));
        char* info_log = (char*)alloca(sizeof(char) * log_length);
        GL_CALL(glGetProgramInfoLog(program, log_length, NULL, info_log));
        LOG_E("Shader.make_program", "Info Log: %s", info_log);
        GL_CALL(glDeleteProgram(program));
        return 0;
    }

    return program;
}

Shader::~Shader()
{
    GL_CALL(glDeleteShader(this->gl_vertex_shader_id));
    GL_CALL(glDeleteShader(this->gl_fragment_shader_id));
    GL_CALL(glDeleteProgram(this->gl_program_id));
}
