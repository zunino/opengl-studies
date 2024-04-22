/**
 * Goal: render some objects using an orthographic projection, as would be the
 * case for a typical 2D game or a GUI.
 *
 * Andre Zunino <neyzunino@gmail.com>
 * Created 21 April 2021
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

GLuint set_up_texture(std::string_view img_path) {
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
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    /* Set texture parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Use loaded image data to make up the new texture */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, img_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    /* Free previously allocated memory for image data */
    stbi_image_free(img_data);

    return texture;
}

struct TexturizedGeometry {
    Geometry geometry;
    GLuint texture;

    void draw() {
        glBindTexture(GL_TEXTURE_2D, this->texture);
        geometry.draw();
    }

    void del() {
        geometry.del();
    }
};

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

    window = glfwCreateWindow(WIDTH, HEIGHT, "Orthographic Projection", NULL, NULL);
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
        {"in_color", "projection"}
        /* {"in_color", "model", "view", "projection"} */
    };

    Geometry b1_geo{
        {
            // positions       // texture coordinates
            -0.6f, 1.0f, 0.0f,  1.0f, 1.0f,  // top right
            -0.6f, 0.6f, 0.0f,  1.0f, 0.0f,  // bottom right
            -1.0f, 0.6f, 0.0f,  0.0f, 0.0f,  // bottom left
            -1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // top left
        },
        {
            0, 1, 3,
            1, 2, 3,
        }
    };

    Geometry b2_geo{
        {
            // positions       // texture coordinates
            1.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // top right
            1.0f, 0.6f, 0.0f,  1.0f, 0.0f,  // bottom right
            0.6f, 0.6f, 0.0f,  0.0f, 0.0f,  // bottom left
            0.6f, 1.0f, 0.0f,  0.0f, 1.0f,  // top left
        },
        {
            0, 1, 3,
            1, 2, 3,
        }
    };

    Geometry b3_geo{
        {
            // positions       // texture coordinates
            -0.6f, -0.6f, 0.0f,  1.0f, 1.0f,  // top right
            -0.6f, -1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // bottom left
            -1.0f, -0.6f, 0.0f,  0.0f, 1.0f,  // top left
        },
        {
            0, 1, 3,
            1, 2, 3,
        }
    };

    Geometry b4_geo{
        {
            // positions       // texture coordinates
            1.0f, -0.6f, 0.0f,  1.0f, 1.0f,  // top right
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
            0.6f, -1.0f, 0.0f,  0.0f, 0.0f,  // bottom left
            0.6f, -0.6f, 0.0f,  0.0f, 1.0f,  // top left
        },
        {
            0, 1, 3,
            1, 2, 3,
        }
    };

    /* I'd like my textures unflipped, please! */
    stbi_set_flip_vertically_on_load(true);

    GLuint b1_texture = set_up_texture("../tex/1.png");
    GLuint b2_texture = set_up_texture("../tex/2.png");
    GLuint b3_texture = set_up_texture("../tex/3.png");
    GLuint b4_texture = set_up_texture("../tex/4.png");

    /* glBindTexture(GL_TEXTURE_2D, b1_texture); */

    auto b1 = TexturizedGeometry{b1_geo, b1_texture};
    auto b2 = TexturizedGeometry{b2_geo, b2_texture};
    auto b3 = TexturizedGeometry{b3_geo, b3_texture};
    auto b4 = TexturizedGeometry{b4_geo, b4_texture};

    /* Activate linked program */
    shader_program.use();

    /* Model */
    /* glm::mat4 model = glm::mat4{1.0f}; */
    /* model = glm::rotate(model, glm::radians(-55.0f), glm::vec3{1.0f, 0.0f, 0.0f}); */

    /* View */
    /* glm::mat4 view = glm::mat4{1.0f}; */
    /* view = glm::translate(view, glm::vec3{0.0f, 0.0f, 0.0f}); */

    /* Projection */
    glm::mat4 projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Drawing code */
        b1.draw();
        b2.draw();
        b3.draw();
        b4.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Deallocate objects */
    b4_geo.del();
    b3_geo.del();
    b2_geo.del();
    b1_geo.del();

    shader_program.del();

    glfwTerminate();
    
    return 0;
}
