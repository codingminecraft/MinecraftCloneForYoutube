// Vertex Shader
#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;

// NOTE: These attributes are not required for the challenges
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 fTexCoords;

// NOTE: These two variables are not required for the challenges
out vec3 fNormal;
out mat3 fTBN;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    fTexCoords = aTexCoords;

    // NOTE: This block is not required for the challenges
    fNormal = aNormal;
    vec3 T = normalize(vec3(vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(vec4(aNormal,    0.0)));
    fTBN = mat3(T, B, N);

    gl_Position = uProjection * uView * (uTransform * vec4(aPosition, 1.0));
}
