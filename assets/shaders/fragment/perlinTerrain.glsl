#version 460 core
out vec4 FragColor;

layout (location = 0) in vec3 fragCoord;
layout (location = 1) in vec3 fNormal;

vec3 lightColor = vec3(0.9, 0.9, 0.9);
vec3 lightPos = vec3(3.0, 30.0, 3.0);

void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(lightPos - fragCoord);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	FragColor = vec4(diffuse, 1);
}
