#include <array>

#include <shader_prog.hpp>

struct Geometry {
    Geometry(
            std::initializer_list<float> vertices,
            std::initializer_list<int> indices);

    void draw();
    void del();

    std::size_t n_indices;

    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;
};

