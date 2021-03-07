/**
 * I guess this is 'Hello, quads!'
 *
 * 3 March 2021
 * Switching to using EBO (Element Buffer Objects) and indexed
 * drawing instead of triangle strips. This also means the drawing calls will
 * be made with glDrawElements instead of glDrawArrays.
 *
 * 5 March 2021
 * Implemented basic ShaderProgram abstraction.
 * Implemented basic Geometry abstraction.
 * 
 * Andre Zunino <neyzunino@gmail.com>
 * Created 24 February 2021
 * Modified 5 March 2021
 */

#include <iostream>

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <shader_prog.hpp>
#include <geometry.hpp>

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

    ShaderProgram shader_program{
        "shaders/vertex.shader",
        "shaders/fragment.shader",
        {"in_color", "transform"}
    };

    Geometry quad1{
        {
           -0.7f, -0.2f, 0.0f,
           -0.7f,  0.7f, 0.0f,
            0.3f, -0.2f, 0.0f,
            0.3f,  0.7f, 0.0f,
        },
        {
            0, 1, 2,
            3, 1, 2,
        },
        { 0.8f, 0.0f, 0.0f, 1.0f },
        shader_program
    };


    Geometry quad2{
        {
           -0.3f, -0.5f, 0.0f,
           -0.3f,  0.5f, 0.0f,
            0.7f, -0.5f, 0.0f,
            0.7f,  0.5f, 0.0f,
        },
        {
            0, 1, 2,
            3, 1, 2,
        },
        { 0.0f, 0.8f, 0.0f, 1.0f },
        shader_program
    };

    Geometry quad3{
        {
           -0.5f, -0.4f, 0.0f,
           -0.5f,  0.4f, 0.0f,
            0.3f, -0.4f, 0.0f,
            0.5f,  0.4f, 0.0f,
        },
        {
            0, 1, 2,
            3, 1, 2,
        },
        { 0.0f, 0.0f, 0.8f, 1.0f },
        shader_program
    };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Activate linked program */
        shader_program.use();

        /* Drawing code */
        quad1.draw();
        quad2.draw();
        quad3.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Deallocate objects */
    quad3.del();
    quad2.del();
    quad1.del();

    shader_program.del();

    glfwTerminate();
    
    return 0;
}
