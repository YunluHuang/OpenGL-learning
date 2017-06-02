#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#include <iostream>

#include "LoadShader.hpp"
#include "Scene.hpp"

#define BUFFER_OFFSET(x) ((const void *)(x))
using namespace std;

//enum VAO_IDs {Triangles, NumVAOs};
//enum Buffer_IDs {ArrayBuffer, NumBuffers};

//
//GLuint VAOs[NumVAOs];
//GLuint Buffers[NumBuffers];
//
//const GLuint NumVertices = 6;

void init() {
//    glGenVertexArrays(NumVAOs, VAOs);
//    glBindVertexArray(VAOs[Triangles]);
    
//    GLfloat vertices[NumVertices][2] = {
//        {-0.9f, -0.9f},
//        {0.85f, -0.9f},
//        {-0.9f, 0.85f},
//        {0.9f, -0.85f},
//        {0.9f, 0.9f},
//        {-0.85f, 0.9f}
//    };
    
//    glGenBuffers(NumBuffers, Buffers);
//    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
//                 GL_STATIC_DRAW);
    
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert"},
        {GL_FRAGMENT_SHADER, "triangles.frag"},
        {GL_NONE, NULL}
    };
    
    GLuint program = LoadShaders(shaders);
    glUseProgram(program);
    
//    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
//    glEnableVertexAttribArray(vPosition);
}

void display() {
//    cout << "display() called" << endl;
//    glClear(GL_COLOR_BUFFER_BIT);
//    
//    glBindVertexArray(VAOs[0]);
//    glDrawArrays(GL_TRIANGLES, 0, );
    
    glFlush();
}

int main(int argc, char * *argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA);
    glutInitWindowSize(512, 512);
    glutCreateWindow(argv[0]);
    
    cout << "GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    
    init();
    
    glutDisplayFunc(display);
    
    glutMainLoop();
}
