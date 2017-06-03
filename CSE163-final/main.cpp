
#include "LoadShader.hpp"
#include "Geometry.hpp"
#include "Control.hpp"

#define MAINPROGRAM
#include "ReadFile.hpp"
#include "variables.h"

#include "math.hpp"

using namespace std;

//initialize the window size
int width = 512;
int height = 512;
float zNear = 0.1f;
float zFar = 100.0f;
float fovy = 75.0f;

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

void printMat4(mat4 & m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << m[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

void init() {
    genBuffers();
    
    //compute the camera view
    model = mat4(1.0f);
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float) width / (float) height, zNear, zFar);
    
    //initialize mouse
    glutWarpPointer(width / 2, height / 2);
    
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert"},
        {GL_FRAGMENT_SHADER, "triangles.frag"},
        {GL_NONE, NULL}
    };
    
    program = loadShaders(shaders);
    glUseProgram(program);
    
    ambientPosition = glGetUniformLocation(program, "ambient");
    specularPosition = glGetUniformLocation(program, "specular");
    diffusePosition = glGetUniformLocation(program, "diffuse");
    shininessPosition = glGetUniformLocation(program, "shininess");
    
    initAllMeshes();
}

void display() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    view = glm::lookAt(eye, center, up);
    mvpPos = glGetUniformLocation(program, "MVP");
    
    for (int i = 0; i < objects.size(); i++) {
        
        // Setup mvp
        mvp = projection * view * objects[i]->transf;
        glUniformMatrix4fv(mvpPos, 1, GL_FALSE, &mvp[0][0]);
        
        //
        glUniform3f(ambientPosition, objects[i]->ambient[0], objects[i]->ambient[1], objects[i]->ambient[2]);
        glUniform3f(diffusePosition, objects[i]->diffuse[0], objects[i]->diffuse[1], objects[i]->diffuse[2]);
        glUniform3f(specularPosition, objects[i]->specular[0], objects[i]->specular[1], objects[i]->specular[2]);
        glUniform1f(shininessPosition, objects[i]->shininess);
        
        // Display the object
        displayObject(objects[i]);
    }
    
    glFlush();
}

void idle() {
    // display();
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
    
    readfile(argv[1]);
    init();
    
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrowKey);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseRotate);
    glutMotionFunc(mouseRotate);
    
    glutMainLoop();
}
