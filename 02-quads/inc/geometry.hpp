#include <array>

#include <shader_prog.hpp>

struct Geometry {
    Geometry(
            std::initializer_list<float> vertices,
            std::initializer_list<int> indices,
            std::array<float, 4> color);
    void draw();
    void draw(ShaderProgram& shader_prog, int in_color_location);
    void del();

    std::array<float, 4> color;
    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;
};

