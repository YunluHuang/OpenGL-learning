#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 modelView;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec3 myNormal;
out vec4 myVertex;
out vec4 shadowCoord;

void main() {
    gl_Position = projection * modelView * vec4(vPosition, 1.0);
    myNormal = vNormal;
    myVertex = vec4(vPosition, 1.0);
    shadowCoord = lightSpaceMatrix * myVertex;
}
