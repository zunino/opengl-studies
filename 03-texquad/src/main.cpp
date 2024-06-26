/**
 * Goal: render a textured quad with a perspective projection
 *
 * Andre Zunino <neyzunino@gmail.com>
 * Created 7 March 2021
 * Modified 7 March 2021
 */

#include <iostream>

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <shader_prog.hpp>
#include <geometry.hpp>

namespace {
    const std::size_t WIDTH = 1024;
    const std::size_t HEIGHT = 768;
}

unsigned int set_up_texture(std::string_view img_path) {
    /* Load image to be used as texture */
    int img_width;
    int img_height;
    int img_channels;
    unsigned char* img_data = stbi_load(
            img_path.data(), &img_width, &img_height, &img_channels, 0);
    if (!img_data) {
        throw std::runtime_error{"Error loading image file"};
    }

    /* Allocate texture */
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    /* Set texture parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Use loaded image data to make up the new texture */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, img_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    /* Free previously allocated memory for image data */
    stbi_image_free(img_data);

    return texture;
}

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

    window = glfwCreateWindow(WIDTH, HEIGHT, "Look! A Textured Quad!", NULL, NULL);
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
        {"in_color", "model", "view", "projection"}
    };

    Geometry quad{
        {
            // positions         // texture coordinates
            0.8f,  0.45f, 0.0f,  1.0f, 1.0f,  // top right
            0.8f, -0.45f, 0.0f,  1.0f, 0.0f,  // bottom right
           -0.8f, -0.45f, 0.0f,  0.0f, 0.0f,  // bottom left
           -0.8f,  0.45f, 0.0f,  0.0f, 1.0f,  // top left
        },
        {
            0, 1, 3,
            1, 2, 3,
        }
    };

    /* I'd like my textures unflipped, please! */
    stbi_set_flip_vertically_on_load(true);

    unsigned int texture = set_up_texture("../tex/texquad.jpeg");
    glBindTexture(GL_TEXTURE_2D, texture);

    /* Activate linked program */
    shader_program.use();

    /* Model */
    glm::mat4 model = glm::mat4{1.0f};
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3{1.0f, 0.0f, 0.0f});

    /* View */
    glm::mat4 view = glm::mat4{1.0f};
    view = glm::translate(view, glm::vec3{0.0f, 0.0f, -2.0f});

    /* Projection */
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float{WIDTH}/float{HEIGHT}, 0.1f, 100.0f);

    int model_location = shader_program.get_uniform_location("model");
    int view_location = shader_program.get_uniform_location("view");
    int projection_location = shader_program.get_uniform_location("projection");

    shader_program.set_uniform_matrix4fv(model_location, model);
    shader_program.set_uniform_matrix4fv(view_location, view);
    shader_program.set_uniform_matrix4fv(projection_location, projection);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Drawing code */
        quad.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Deallocate objects */
    quad.del();

    shader_program.del();

    glfwTerminate();
    
    return 0;
}
