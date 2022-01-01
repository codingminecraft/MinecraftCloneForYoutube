// Vertex Shader
#version 460 core

layout (location = 0) in vec4 aColor;
layout (location = 1) in vec3 aPosition;

out vec4 fColor;

void main()
{
    fColor = aColor;
    gl_Position = vec4(aPosition, 1.0);
}
