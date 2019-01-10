//
// Created by Daniel on 04.12.2018.
//

#ifndef SONAR_APP_SHADER_HPP
#define SONAR_APP_SHADER_HPP

#include <string>

class Shader
{
public:
    Shader();
    Shader(const std::string& vertex_shader_src, const std::string& fragment_shader_src);
    void bind() const;
    void unbind() const;
    int get_attribute_location(const std::string name) const;
    int get_uniform_location(const std::string name) const;
    void set_uniform_matrix4fv_value(int location, const float* matrix) const;
    void set_uniform_2fv_value(int location, const float* vec) const;
    ~Shader();

private:
    unsigned int gl_vertex_shader_id, gl_fragment_shader_id, gl_program_id;

    unsigned int load_shader(unsigned int shader_type, const char* const shader_src);
    unsigned int make_program();
};

#endif //SONAR_APP_SHADER_HPP
