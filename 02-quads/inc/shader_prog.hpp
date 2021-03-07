#ifndef AZ_SHADER_PROGRAM_
#define AZ_SHADER_PROGRAM_

#include <string_view>
#include <unordered_map>
#include <string>

struct ShaderProgram final {
    ShaderProgram(
            std::string_view vertex_shader_path,
            std::string_view fragment_shader_path,
            std::initializer_list<std::string> uniform_names);
    void use() const;
    void del() const;
    void set_uniform_4f(int location, float x, float y, float z, float w) const;
    int get_uniform_location(std::string_view name) const;

    unsigned int id;
    std::unordered_map<std::string, int> uniforms;
};

#endif

