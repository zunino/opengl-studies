#version 330

layout (location=0) in vec3 pos;

uniform mat4 transform;

void main() {
    /* gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f); */
    gl_Position = transform * vec4(pos, 1.0f);
}

