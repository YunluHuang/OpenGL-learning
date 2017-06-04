#version 410 core
//layout(location = 0) in vec3 vPos;
//
//uniform mat4 lightMVP;
////uniform mat4 meshModel;
//
//void main() {
//    gl_Position = lightMVP * vec4(vPos, 1.0);
//}

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;

void main(){
    gl_Position =  depthMVP * vec4(vertexPosition_modelspace,1);
}
