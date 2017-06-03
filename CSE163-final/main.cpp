#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "LoadShader.hpp"
#include "Geometry.hpp"
#include "Control.hpp"

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
float fovy = 60.0f;

//initialize camera position
vec3 eye = EYE;
vec3 up = UP;
vec3 center = CENTER;

//initialize mvp
mat4 model, view, projection, mvp;

//initialize key press rotation amount
float rotateAmount = 1 / PI;

//initialize mouse setting
float moveSpeed = 0.1f, mouseSpeed = 0.1f;
float limitedFPS = 1.0f / 60.0f;

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
    
    //initialize mouse
    glutWarpPointer(width / 2, height / 2);
    
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert"},
        {GL_FRAGMENT_SHADER, "triangles.frag"},
        {GL_NONE, NULL}
    };
    program = loadShaders(shaders);
    glUseProgram(program);
}

void display() {
    
    mvp = projection * view * model;
    mvpPos = glGetUniformLocation(program, "MVP");
    glUniformMatrix4fv(mvpPos, 1, GL_FALSE, &mvp[0][0]);
    
    displayMesh(Meshes[0]);
    displayPrimitive(Triangles);
    glFlush();
}

//void keyboard(unsigned char key, int x, int y) {
//    vec3 xAxis = normalize(cross(eye - center, up));
//    vec3 yAxis = normalize(up);
//    vec3 zAxis = normalize(eye - center);
//    switch (key) {
//        case 'w': {
//            cout << "w pressed, move forward" << endl;
//            view = glm::translate(view, moveSpeed * zAxis);
//            break;
//        }
//        case 's': {
//            cout << "s pressed, move backward" << endl;
//            view = glm::translate(view, moveSpeed * -zAxis);
//            break;
//        }
//        case 'a': {
//            cout << "a pressed, move left" << endl;
//            view = glm::translate(view, moveSpeed * -xAxis);
//            break;
//        }
//        case 'd': {
//            cout << "d pressed, move right" << endl;
//            view = glm::translate(view, moveSpeed * xAxis);
//            break;
//        }
//        case 27: {
//            exit(1);
//            break;
//        }
//        default: {
//            cout << "invalid key" << endl;
//            break;
//        }
//    }
//    glutPostRedisplay();
//}

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
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMove);
    glutMotionFunc(mouseRotate);
    
    glutMainLoop();
}
