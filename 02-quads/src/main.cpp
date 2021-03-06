/**
 * I guess this is 'Hello, quads!'
 *
 * Switching to using EBO (Element Buffer Objects) and indexed
 * drawing instead of triangle strips. This also means the drawing calls will
 * be made with glDrawElements instead of glDrawArrays.
 * 
 * Andre Zunino <neyzunino@gmail.com>
 * Created 24 February 2021
 * Modified 3 March 2021
 */

#include <iostream>

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <shader_prog.hpp>

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Trying to set OpenGL version (must be before window creation) */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;

    window = glfwCreateWindow(640, 480, "Hello Quads", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cerr << "Problem initializing glad\n";
        glfwTerminate();
        return -1;
    }

    ShaderProgram shader_prog{"shaders/vertex.shader", "shaders/fragment.shader"};

    /* Get location of uniform 'color_in' */
    int in_color_location = glGetUniformLocation(shader_prog.id, "in_color");

    /* Vertex data */
    float quad1[] = {
        -0.7f, -0.2f, 0.0f,
        -0.7f,  0.7f, 0.0f,
         0.3f, -0.2f, 0.0f,
         0.3f,  0.7f, 0.0f,
    };

    /* Vertex indexing for quad1 */
    unsigned int indices1[] = {
        0, 1, 2,
        3, 1, 2,
    };

    float quad2[] = {
        -0.3f, -0.5f, 0.0f,
        -0.3f,  0.5f, 0.0f,
         0.7f, -0.5f, 0.0f,
         0.7f,  0.5f, 0.0f,
    };

    /* Vertex buffer objects (VBO) to store vertex data in GPU's memory */
    unsigned int vbo1, vbo2;
    glGenBuffers(1, &vbo1);
    glGenBuffers(1, &vbo2);

    /* Vertex attribute object (VAO) */
    unsigned int vao1, vao2;
    glGenVertexArrays(1, &vao1);
    glGenVertexArrays(1, &vao2);

    /* Element buffer object (EBO) */
    unsigned int ebo1;
    glGenBuffers(1, &ebo1);

    /* Bind vao1, then vbo1 and copy vertex data for quad1 */
    glBindVertexArray(vao1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof quad1, quad1, GL_STATIC_DRAW);

    /* Bind ebo1 and copy indices; ebo1 will be recalled by vao1 */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices1, indices1, GL_STATIC_DRAW);

    /* Tell OpenGL how it should interpret vertex data */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Bind vao2, then vbo2 and copy vertex data for quad2 */
    glBindVertexArray(vao2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof quad2, quad2, GL_STATIC_DRAW);

    /* Again for vao2, which has just been bound */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Unbind VAO */
    glBindVertexArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Activate linked program */
        shader_prog.use();

        /* Set uniform 'in_color' value */
        shader_prog.set_uniform_4f(in_color_location, 0.4f, 0.1f, 0.05f, 1.0f);

        /* Fill mode */
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        /* Drawing code */
        glBindVertexArray(vao1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Set uniform 'in_color' value */
        shader_prog.set_uniform_4f(in_color_location, 0.8f, 0.7f, 0.05f, 1.0f);

        /* Wireframe mode */
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(vao2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Deallocate objects */
    glDeleteVertexArrays(1, &vao1);
    glDeleteVertexArrays(1, &vao2);
    glDeleteBuffers(1, &vbo1);
    glDeleteBuffers(1, &vbo2);

    shader_prog.del();

    glfwTerminate();
    
    return 0;
}
