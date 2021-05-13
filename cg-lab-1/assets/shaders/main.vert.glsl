#version 420 core
layout (location = 0) in vec2 inPos;
uniform float scale;
uniform vec2  offset;

void main() {
    vec2 resultingPos;
    resultingPos = inPos * scale + offset;
    gl_Position = vec4(resultingPos, 0.0, 1.0);
}
