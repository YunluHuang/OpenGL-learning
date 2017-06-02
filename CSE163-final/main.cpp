#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#include <iostream>

#include "LoadShader.hpp"
#include "Geometry.hpp"

#define MAINPROGRAM
#include "ReadFile.hpp"

#define BUFFER_OFFSET(x) ((const void *)(x))
using namespace std;

void init() {
    genBuffers();
    initMesh(Meshes[0]);
    
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert"},
        {GL_FRAGMENT_SHADER, "triangles.frag"},
        {GL_NONE, NULL}
    };
    GLuint program = loadShaders(shaders);
    glUseProgram(program);
}

void display() {
    displayMesh(Meshes[0]);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(1);
            break;
        default:
            cout << "invalid key" << endl;
            break;
    }
}

int main(int argc, char * *argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA);
    glutInitWindowSize(512, 512);
    glutCreateWindow(argv[0]);
    
    cout << "GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    
    readfile();
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
}
