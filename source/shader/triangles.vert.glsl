#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 modelView;
uniform mat4 projection;

out vec3 myNormal;
out vec4 myVertex;

void main() {
    gl_Position = projection * modelView * vec4(vPosition, 1.0);
    myNormal = vNormal;
    myVertex = vec4(vPosition, 1.0);
}

//#version 410 core
//// Inputs to the vertex shader
//layout (location = 0) in vec3 vPosition;
//layout (location = 1) in vec3 vNormal;
//
//// Uniform variables
//uniform mat4 MVP;
//uniform mat4 projection;
//
//// Additional outputs for the vertex shader in addition to gl_Position
//out vec3 mynormal;
//out vec4 myvertex;
//
//void main() {
//    gl_Position = projection * modelview * vec4(position, 1.0f);
//    // Forward these vectors to the fragment shader
//    mynormal = normal;
//    myvertex = vec4(position, 1.0f);
//}
