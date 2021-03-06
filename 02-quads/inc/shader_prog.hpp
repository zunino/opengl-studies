#ifndef AZ_SHADER_PROGRAM_
#define AZ_SHADER_PROGRAM_

#include <string_view>

struct ShaderProgram final {
    ShaderProgram(
            std::string_view vertex_shader_path,
            std::string_view fragment_shader_path);
    void use();
    void del();
    void set_uniform_4f(std::string_view name, float x, float y, float z, float w);
    void set_uniform_4f(int uniform_location, float x, float y, float z, float w);

    unsigned int id;
};

#endif

