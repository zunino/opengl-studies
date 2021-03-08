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
#include <random>

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

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
        }
    };

    /* Activate linked program */
    shader_program.use();

    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<float> trans_dist{-1.0f, 1.0f};
    std::uniform_real_distribution<float> scale_dist{0.1f, 1.2f};
    std::uniform_real_distribution<float> color_dist{0.0f, 1.0f};

    auto random_transform = [&]() {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(
                    trans_dist(gen), trans_dist(gen), trans_dist(gen)));
        transform = glm::scale(transform, glm::vec3(
                    scale_dist(gen), scale_dist(gen), scale_dist(gen)));
        int transform_location = shader_program.get_uniform_location("transform");
        shader_program.set_uniform_matrix4fv(transform_location, transform);

        int in_color_location = shader_program.get_uniform_location("in_color");
        shader_program.set_uniform_4f(in_color_location,
                color_dist(gen), color_dist(gen), color_dist(gen), 1.0f);
    };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Drawing code */
        random_transform();
        quad1.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Deallocate objects */
    quad1.del();

    shader_program.del();

    glfwTerminate();
    
    return 0;
}
