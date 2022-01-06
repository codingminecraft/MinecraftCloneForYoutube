// Vertex Shader
#version 460 core
layout (location = 0) in vec3 aPosition;

layout (location = 0) out vec2 fragCoord;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    fragCoord = aPosition.xy + vec2(0.5);
    gl_Position = uProjection * uView * (uTransform * vec4(aPosition, 1.0));
}
