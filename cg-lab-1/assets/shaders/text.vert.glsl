#version 420 core

layout (location = 0) in vec4 inMixed;
out vec2 texCoords;
uniform float scale;
uniform vec2  offset;

void main() {
    vec2 finalPos = inMixed.xy * scale + offset;
    gl_Position = vec4(finalPos, 0.0f, 1.0f);

    texCoords = inMixed.zw;
}
