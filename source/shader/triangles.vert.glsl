#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int lightAmount;
uniform mat4 lightSpaceMatrices[10];

out vec3 myNormal;
out vec4 myVertex;
out vec4 shadowCoords[10];

void main() {
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    myNormal = vNormal;
    myVertex = vec4(vPosition, 1.0);
    
    for (int i = 0; i < lightAmount; i++) {
        shadowCoords[i] = lightSpaceMatrices[i] * model * myVertex;
    }
}
