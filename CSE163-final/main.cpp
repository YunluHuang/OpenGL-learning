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
float fovy = 45.0f;

//initialize camera position
vec3 eye = EYE;
vec3 up = UP;
vec3 center = CENTER;

//initialize mvp
mat4 model, view, projection, mvp;

//initialize key press rotation amount
float rotateAmount = 1 / PI;

//initialize mouse setting
int oldX, oldY;
float speed = 3.0f, mouseSpeed = 0.05f;
float limitedFPS = 1.0f / 60.0f;

//initialize the programID mvpPos
GLuint program;
GLuint mvpPos;

void init() {
    genBuffers();
    
    //compute the camera view
    model = mat4(1.0f);
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear, zFar);
    
    //initialize mouse
    oldX = width / 2;
    oldY = height / 2;
    glutWarpPointer(oldX, oldY);
    
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert"},
        {GL_FRAGMENT_SHADER, "triangles.frag"},
        {GL_NONE, NULL}
    };
    
    program = loadShaders(shaders);
    glUseProgram(program);
    
    initAllMeshes();
}

void display() {
    
    mvp = projection * view * model;
    mvpPos = glGetUniformLocation(program, "MVP");
    glUniformMatrix4fv(mvpPos, 1, GL_FALSE, &mvp[0][0]);
    
    for (int i = 0; i < objects.size(); i++) {
        displayMesh(objects[i]);
    }
    
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
            model = glm::rotate(model, rotateAmount, up);
            break;
        }
        //up
        case 101: {
            //rotate vertically counter-clockwise
            cout << "up arrow key" << endl;
            model = glm::rotate(model, rotateAmount, cross(eye - center, up));
            break;
        }
        //right
        case 102: {
            //rotate horizontally counter-clockwise
            cout << "right arrow key" << endl;
            model = glm::rotate(model, -rotateAmount, up);
            break;
        }
        //down
        case 103: {
            //rotate vertically counter-clockwise
            cout << "down arrow key" << endl;
            model = glm::rotate(model, -rotateAmount, cross(eye - center, up));
            break;
        }
        default: {
            break;
        }
    }
    glutPostRedisplay();
}

void mouseDrag(int x, int y) {
    //press left button
    cout << "oldx = " << oldX << ", x = " << x << endl;
    float rotateX = mouseSpeed * limitedFPS * float (x - oldX);
    float rotateY = mouseSpeed * limitedFPS * float (y - oldY);
    vec3 eulerAngle = vec3(rotateY, rotateX, 0.0f);
    quat quaternion = quat(eulerAngle);
    mat4 rotationM = glm::toMat4(quaternion);
    view = rotationM * view;
    oldX = x;
    oldY = y;
    glutPostRedisplay();
}

void mouseMove(int x, int y) {
//    float rotateX = mouseSpeed * limitedFPS * float (oldX - x);
//    float rotateY = mouseSpeed * limitedFPS * float (oldY - y);
//    vec3 eulerAngle = vec3(rotateX, rotateY, 0.0f);
//    quat quaternion = quat(eulerAngle);
//    mat4 rotationM = glm::toMat4(quaternion);
//    model = rotationM * model;
    oldX = x;
    oldY = y;
    glutPostRedisplay();
}

int main(int argc, char * argv[]) {
    
    if (argc < 2) {
        cerr << "Please specify the scene file" << endl;
//        return -1;
    }
    
    glutInit(&argc, argv);
    
    //create the window
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("final-project");
    
    cout << "GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    
    readfile(argv[1]);
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrowKey);
    glutPassiveMotionFunc(mouseDrag);
    glutMotionFunc(mouseDrag);
    
    glutMainLoop();
}
