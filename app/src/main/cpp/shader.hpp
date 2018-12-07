//
// Created by Daniel on 04.12.2018.
//

#ifndef SONAR_APP_SHADER_HPP
#define SONAR_APP_SHADER_HPP

#include <string>

class Shader
{
public:
    Shader(std::string vertex_shader_src, std::string fragment_shader_src);
    void bind() const;
    void unbind() const;
    ~Shader();

private:
    unsigned int gl_vertex_shader_id, gl_fragment_shader_id, gl_program_id;

    unsigned int load_shader(unsigned int shader_type, const char* const shader_src);
    unsigned int make_program();
};

#endif //SONAR_APP_SHADER_HPP
