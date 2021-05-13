#version 420 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float rotation_angle;
uniform mat3  rotation_look_at;

out vec3 real_pos;
out vec3 color;
out vec3 real_normal;

void main()
{    
    float radius = 1;
    vec3 rotation = vec3(radius * cos(rotation_angle),
                         0,
                         radius * sin(rotation_angle));
    rotation = rotation_look_at * rotation;

    real_pos    = vec3(model * vec4(in_pos + rotation, 1.0f));
    real_normal = normalize(mat3(model) * normal);
    color       = in_color;

    gl_Position = proj * view * vec4(real_pos, 1.0f);
}
