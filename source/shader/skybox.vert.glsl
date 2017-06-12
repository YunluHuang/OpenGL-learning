#version 410 core
layout(location = 0) in vec3 vPos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    texCoords = vPos;
    gl_Position = projection * view * vec4(vPos, 1.0);
}