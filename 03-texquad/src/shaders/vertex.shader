#version 330

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;

out vec2 tex_coord;

/* uniform mat4 transform; */

void main() {
    /* gl_Position = transform * vec4(pos, 1.0f); */
    gl_Position = vec4(pos, 1.0f);
    tex_coord = tex;
}

