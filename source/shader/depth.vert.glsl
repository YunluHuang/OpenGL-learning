#version 410 core
layout(location = 0) in vec3 vPos;

uniform mat4 lightProjectionView;
uniform mat4 lightModel;

void main() {
    gl_Position = lightProjectionView * lightModel * vec4(vPos, 1.0);
}
