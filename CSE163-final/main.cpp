#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "LoadShader.hpp"
#include "Geometry.hpp"

#define MAINPROGRAM
#include "ReadFile.hpp"
#include "variables.h"

#define BUFFER_OFFSET(x) ((const void *)(x))
#define PI 3.1415926f
#define EYE vec3(0.0,0.0,5.0)
#define UP vec3(0.0,1.0,0.0)
#define CENTER vec3(0.0,0.0,0.0)

using namespace std;

//initialize the window size
int width = 512;
int height = 512;
float zNear = 0.1f;
float zFar = 100.0f;
float fovy = 90.0f;

//initialize camera position
vec3 eye = EYE;
vec3 up = UP;
vec3 center = CENTER;

//initialize key press rotation amount
mat4 model, view, projection, mvp;
float rotateAmount = 1 / PI;

//initialize the programID mvpPos
GLuint program;
GLuint mvpPos;

void init() {
    genBuffers();
    initMesh(Meshes[0]);
    initPrimitive(Triangles);
    
    //compute the camera view
    model = mat4(1.0f);
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear, zFar);
    mvp = projection * view * model;
    
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert"},
        {GL_FRAGMENT_SHADER, "triangles.frag"},
        {GL_NONE, NULL}
    };
    program = loadShaders(shaders);
    glUseProgram(program);
}

void display() {
    

    
    mvpPos = glGetUniformLocation(program, "MVP");
    glUniformMatrix4fv(mvpPos, 1, GL_FALSE, &mvp[0][0]);
    
    displayMesh(Meshes[0]);
    displayPrimitive(Triangles);
    glFlush();
}

/**
 handle keyboard press
*/
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: {
            exit(1);
            break;
        }
        default: {
            cout << "invalid key" << endl;
            break;
        }
    }
    glutPostRedisplay();
}

/** 
 handle arrow key press
*/
void arrowKey(int key, int x, int y) {
    switch (key) {
        //left
        case 100: {
            //rotate horizontally counter-clockwise
            cout << "left arrow key" << endl;
            mvp = glm::rotate(mvp, rotateAmount, up);
            break;
        }
        //up
        case 101: {
            //rotate vertically counter-clockwise
            cout << "up arrow key" << endl;
            mvp = glm::rotate(mvp, rotateAmount, cross(eye - center, up));
            break;
        }
        //right
        case 102: {
            //rotate horizontally counter-clockwise
            cout << "right arrow key" << endl;
            mvp = glm::rotate(mvp, -rotateAmount, up);
            break;
        }
        //down
        case 103: {
            //rotate vertically counter-clockwise
            cout << "down arrow key" << endl;
            mvp = glm::rotate(mvp, -rotateAmount, cross(eye - center, up));
            break;
        }
        default: {
            break;
        }
    }
    glutPostRedisplay();
}

int main(int argc, char * *argv) {
    
    glutInit(&argc, argv);
    
    //create the window
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("final-project");
    
    cout << "GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    
    readfile();
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrowKey);
    
    glutMainLoop();
}
