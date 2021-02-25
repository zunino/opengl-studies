/**
 * Playing with OpenGL after many, many years (decades?)
 * 
 * Source: https://learnopengl.com/Getting-started/Hello-Triangle
 * 
 * Andre Zunino <neyzunino@gmail.com>
 * Created 9 February 2021
 * Modified 24 February 2021
 */

#include <iostream>

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

const char* const vertex_shader_src =
    "#version 330\n"
    "layout (location=0) in vec3 pos;\n"
    "void main() {\n"
    "  gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);\n"
    "}\n";

const char* const fragment_shader_src =
    "#version 330\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

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

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    /* Vertex shader */
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
    glCompileShader(vertex_shader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << '\n';
        // clean up and exit?!
    }

    /* Fragment shader */
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << '\n';
    }

    /* Shader program */
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_program, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << '\n';
    }

    /* Get rid of shaders since they've already been used */
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    /* Vertex data */
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    /* Vertex buffer object (VBO) to store vertex data in GPU's memory */
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    /* Creating vertex attribute object (VAO) */
    unsigned int vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    /* Tell OpenGL how it should interpret vertex data */
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
        glUseProgram(shader_program);

        glBindVertexArray(vao);
        /* Drawing code */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Deallocate objects */
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}
