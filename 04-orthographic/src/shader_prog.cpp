#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shader_prog.hpp>

using namespace std::string_literals;

std::string load_shader_src(std::string_view src_path) {
    std::ifstream src_file{src_path.data()};
    if (!src_file) {
        throw std::runtime_error("Shader path not found: '"s + src_path.data() + "'");
    }
    std::stringstream src_text;
    src_text << src_file.rdbuf();
    src_file.close();
    return src_text.str();
}

GLuint compile_shader(const std::string& shader_src, GLenum gl_shader_type) {
    GLuint shader_id = glCreateShader(gl_shader_type);

    const char* const shader_code = shader_src.c_str();
    glShaderSource(shader_id, 1, &shader_code, nullptr);

    int success;
    char info_buffer[512];

    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader_id, 512, nullptr, info_buffer);
        throw std::runtime_error("Shader "s + std::to_string(shader_id) + " compilation failed");
    }

    return shader_id;
}

ShaderProgram::ShaderProgram(
        std::string_view vert_shader_path,
        std::string_view frag_shader_path,
        std::initializer_list<std::string> uniform_names) {

    std::string vert_shader_src = load_shader_src(vert_shader_path);
    GLuint vert_shader_id = compile_shader(vert_shader_src, GL_VERTEX_SHADER);

    std::string frag_shader_src = load_shader_src(frag_shader_path);
    GLuint frag_shader_id = compile_shader(frag_shader_src, GL_FRAGMENT_SHADER);

    GLuint shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vert_shader_id);
    glAttachShader(shader_program_id, frag_shader_id);
    glLinkProgram(shader_program_id);

    int success;
    char info_buffer[512];
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_program_id, 512, nullptr, info_buffer);
        throw std::runtime_error("Shader program linking failed: "s + info_buffer);
    }

    glDeleteShader(frag_shader_id);
    glDeleteShader(vert_shader_id);

    this->id = shader_program_id;

    /* Get and store location of uniforms */
    for (auto uniform_name : uniform_names) {
        this->uniforms[uniform_name] = glGetUniformLocation(this->id, uniform_name.data());
    }
}

void ShaderProgram::use() const {
    glUseProgram(this->id);
}

void ShaderProgram::del() const {
    glDeleteProgram(this->id);
}

int ShaderProgram::get_uniform_location(std::string_view name) const {
    auto iter = this->uniforms.find(name.data());
    if (iter == std::end(this->uniforms)) {
        throw std::invalid_argument{"No such uniform found"};
    }
    return iter->second;
}

void ShaderProgram::set_uniform_4f(int location, float x, float y, float z, float w) const {
    glUniform4f(location, x, y, z, w);
}

void ShaderProgram::set_uniform_matrix4fv(int location, const glm::mat4& transform) const {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));
}

