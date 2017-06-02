#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#include <iostream>

#include "LoadShader.hpp"
#include "Geometry.hpp"

#define BUFFER_OFFSET(x) ((const void *)(x))
using namespace std;

void init() {
    char sampleFile[] = "teapot.obj";
    initGeometry(sampleFile);
    
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert"},
        {GL_FRAGMENT_SHADER, "triangles.frag"},
        {GL_NONE, NULL}
    };
    GLuint program = loadShaders(shaders);
    glUseProgram(program);
}

void display() {
    displayGeometry();
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
