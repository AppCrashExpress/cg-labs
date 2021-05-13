#version 420 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 real_pos;
out vec3 color;
out vec3 real_normal;

void main()
{
    real_pos    = vec3(model * vec4(in_pos, 1.0f));
    real_normal = normalize(mat3(model) * normal);
    color       = in_color;

    gl_Position = proj * view * vec4(real_pos, 1.0f);
}
