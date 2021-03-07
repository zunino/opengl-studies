#include <iostream>

#include <glad/glad.h>

#include <geometry.hpp>

Geometry::Geometry(
        std::initializer_list<float> vertices,
        std::initializer_list<int> indices,
        std::array<float, 4> color,
        ShaderProgram& shader_program) 
    : n_indices{indices.size()}, color{color}, shader_program{shader_program} {

    /* Vertex buffer object (VBO) to store vertex data in GPU memory */
    glGenBuffers(1, &this->vbo);

    /* Vertex attribute object (VAO) */
    glGenVertexArrays(1, &this->vao);

    /* Element buffer object (EBO) */
    glGenBuffers(1, &this->ebo);

    /* Bind vao */
    glBindVertexArray(this->vao);

    /* Bind vbo and copy vertex data */
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    std::size_t vertices_size = sizeof *std::begin(vertices) * vertices.size();
    glBufferData(GL_ARRAY_BUFFER, vertices_size, std::begin(vertices), GL_STATIC_DRAW);

    /* Bind ebo and copy indices; ebo will be recalled by previously-bound vao */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    std::size_t indices_size = sizeof *std::begin(indices) * n_indices;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, std::begin(indices), GL_STATIC_DRAW);

    /* Tell OpenGL how it should interpret vertex data */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Unbind vao */
    glBindVertexArray(0);
}

void Geometry::draw() {
    shader_program.set_uniform_4f(
            shader_program.in_color_location,
            color[0], color[1], color[2], color[3]);
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->n_indices, GL_UNSIGNED_INT, 0);
}

void Geometry::del() {
    glDeleteBuffers(1, &this->ebo);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

