#version 410 core
layout(location = 0) in vec3 vPos;

uniform mat4 lightProjectionView;
uniform mat4 meshModel;

void main() {
    gl_Position = lightProjectionView * meshModel * vec4(vPos, 1.0);
}
