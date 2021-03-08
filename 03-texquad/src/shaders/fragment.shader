#version 330

in vec2 TexCoord;

uniform sampler2D texture_sampler;

out vec4 color;

void main() {
    color = texture(texture_sampler, TexCoord);
}

