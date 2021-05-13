#version 420 core

in  vec2 texCoords;
out vec4 color;
uniform sampler2D glyph;

void main() {
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(glyph, texCoords).r);
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f) * sampled;
}