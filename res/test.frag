#version 330 core

in vec2 v_texcoord;

out vec4 fragColor;

void main() {
    fragColor = vec4(v_texcoord, 1.0, 1.0);
}
