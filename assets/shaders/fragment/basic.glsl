// Fragment shader
#version 460 core

out vec4 FragColor;

layout (location = 0) in vec2 fragCoord;

uniform vec3 uColor;

void main()
{
    FragColor = vec4(uColor, 1.0);
}
