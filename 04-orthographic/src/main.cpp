/**
 * Goal: render some objects using an orthographic projection, as would be the
 * case for a typical 2D game or a GUI.
 *
 * Andre Zunino <neyzunino@gmail.com>
 * Created 21 April 2024
 * Last modified 22 April 2024
 */

#include <iostream>
#include <vector>
#include <memory>

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

struct Square {
    std::shared_ptr<Geometry> geometry;
    GLuint texture;
    glm::mat4 transformation;
    const ShaderProgram& shader;
    GLint model_location;

    void draw() {
        glBindTexture(GL_TEXTURE_2D, this->texture);
        shader.set_uniform_matrix4fv(model_location, transformation);
        geometry->draw();
    }

    void del() {
        geometry->del();
    }

    void rotate(float angle) {
        transformation = glm::rotate(transformation, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    }
};

struct Scene {
    std::vector<std::shared_ptr<Square>> squares;

    void add_square(std::shared_ptr<Geometry> square_geo, GLuint texture,
                    glm::mat4 &&transformation, ShaderProgram &shader_program,
                    GLint model_location) {
      squares.push_back(std::make_shared<Square>(
          square_geo, texture, transformation, shader_program, model_location));
    }

    void del() {
        for (auto&& square : squares) {
            square->del();
        }
    }

    void draw() {
        for (auto&& square : squares) {
            square->draw();
        }
    }
};

Scene scene;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
        auto& sq2 = scene.squares[1];
        sq2->rotate(-8.0f);
    } else if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
        auto& sq2 = scene.squares[1];
        sq2->rotate(8.0f);
    }
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
        {"model"}
        /* {"in_color", "model", "view", "projection"} */
    };

    auto square_geo = std::make_shared<Geometry>(
        std::initializer_list<float>{
            // positions         // texture coordinates
             0.2f,  0.2f, 0.0f,  1.0f, 1.0f,  // top right
             0.2f, -0.2f, 0.0f,  1.0f, 0.0f,  // bottom right
            -0.2f, -0.2f, 0.0f,  0.0f, 0.0f,  // bottom left
            -0.2f,  0.2f, 0.0f,  0.0f, 1.0f,  // top left
        },
        std::initializer_list<int>{
            0, 1, 3,
            1, 2, 3,
        }
    );

    /* I'd like my textures unflipped, please! */
    stbi_set_flip_vertically_on_load(true);

    GLuint sq1_texture = set_up_texture("../tex/1.png");
    GLuint sq2_texture = set_up_texture("../tex/2.png");
    GLuint sq3_texture = set_up_texture("../tex/3.png");
    GLuint sq4_texture = set_up_texture("../tex/4.png");

    glm::mat4 identity = glm::mat4{1.0f};

    glm::mat4 sq1_transform = glm::translate(identity, glm::vec3(-0.4f, 0.0f, 0.0f));

    glm::mat4 sq2_transform = glm::translate(identity, glm::vec3(0.4f, -0.3f, 0.0f));
    sq2_transform = glm::rotate(sq2_transform, glm::radians(-42.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    GLint model_location = shader_program.get_uniform_location("model");

    scene.add_square(square_geo, sq1_texture, std::move(sq1_transform), shader_program, model_location);
    scene.add_square(square_geo, sq2_texture, std::move(sq2_transform), shader_program, model_location);

    /* Activate linked program */
    shader_program.use();

    /* Model */
    /* glm::mat4 model = glm::mat4{1.0f}; */
    /* model = glm::rotate(model, glm::radians(-55.0f), glm::vec3{1.0f, 0.0f, 0.0f}); */

    /* View */
    /* glm::mat4 view = glm::mat4{1.0f}; */
    /* view = glm::translate(view, glm::vec3{0.0f, 0.0f, 0.0f}); */

    /* Projection */
    /* glm::mat4 projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f); */

    glfwSetKeyCallback(window, key_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Drawing code */
        /* square_1.draw(); */
        /* square_2.draw(); */
        scene.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    /* Deallocate objects */
    scene.del();

    shader_program.del();

    glfwTerminate();
    
    return 0;
}
