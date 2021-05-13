#version 420 core
in vec3 passedColor;
out vec4 fragColor;
void main()
{
    fragColor = vec4(passedColor, 1.0);
}
