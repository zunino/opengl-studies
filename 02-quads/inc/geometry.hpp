#include <array>

#include <shader_prog.hpp>

struct Geometry {
    Geometry(
            std::initializer_list<float> vertices,
            std::initializer_list<int> indices,
            std::array<float, 4> color,
            ShaderProgram& shader_program);
    void draw();
    void del();

    std::size_t n_indices;
    std::array<float, 4> color;

    ShaderProgram& shader_program;

    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;
};

