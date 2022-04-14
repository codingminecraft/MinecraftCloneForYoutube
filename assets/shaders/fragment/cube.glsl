// Fragment shader
#version 460 core

out vec4 FragColor;

in vec2 fTexCoords;

// NOTE: These are not required for the challenges
in vec3 fNormal;
in mat3 fTBN;

uniform sampler2D uTexture;

// NOTE: These are not required for the challenges
uniform sampler2D uNormalTexture;
const vec3 sunlightDir = normalize(vec3(-0.3, -0.5, -0.2));
const float ambientOcclusion = 0.3f;

void main()
{
    // NOTE: For the challenge all you need here is something like
    // FragColor = texture(uTexture, fTexCoords);
    // Everything in this shader is just to enable normal surface lighting :)

    vec4 defaultColor = vec4(0.6, 0.1, 0.04, 1.0);
    vec4 albedo = texture(uTexture, fTexCoords);
    float lightStrength = clamp(-dot(fNormal, sunlightDir), 0, 1);
    lightStrength = max(ambientOcclusion, lightStrength);

    vec3 textureNormal = normalize(texture(uNormalTexture, fTexCoords).xyz * 2.0 - 1.0);
    textureNormal = normalize(fTBN * textureNormal);
    float incidentAngle = -dot(textureNormal, sunlightDir);
    float normalLightStrength = clamp(incidentAngle, 0.0, 1.0);
    normalLightStrength = (normalLightStrength * 0.7) + 0.3;

    if (incidentAngle < 0) {
        normalLightStrength -= ((incidentAngle * 0.7) + 0.3);
        normalLightStrength = clamp(normalLightStrength, 0, 1);
    }

    vec4 finalColor = albedo * normalLightStrength;

    FragColor = finalColor;
}
