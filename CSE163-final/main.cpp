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
float rotateAmount = 1 / PI;

//initialize the programID mvpPos
GLuint program;
GLuint mvpPos;

void init() {
    genBuffers();
    
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert"},
        {GL_FRAGMENT_SHADER, "triangles.frag"},
        {GL_NONE, NULL}
    };
    
    program = loadShaders(shaders);
    glUseProgram(program);
    
    unsigned int counter = 0;
    for (auto it = loadedMeshes.begin(); it != loadedMeshes.end(); it++) {
        it->second->id = counter++;
        initMesh(it->second);
    }
}

void display() {
    
    //compute the camera view
    mat4 view = glm::lookAt(eye, center, up);
    mat4 projection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear, zFar);
    mat4 model = mat4(1.0f);
    mat4 mvp = projection * view * model;
    
    mvpPos = glGetUniformLocation(program, "MVP");
    glUniformMatrix4fv(mvpPos, 1, GL_FALSE, &mvp[0][0]);
    
    for (int i = 0; i < objects.size(); i++) {
        displayMesh(objects[i]);
    }
    
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

int main(int argc, char * argv[]) {
    
    if (argc < 2) {
        cerr << "Please specify the scene file" << endl;
        return -1;
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
    
    glutMainLoop();
}
