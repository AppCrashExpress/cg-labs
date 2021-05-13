#version 420 core
layout (location = 0) vec2 position;

void main() {
    gl_Position = vec4(position, 0.0f, 1.0f);
}