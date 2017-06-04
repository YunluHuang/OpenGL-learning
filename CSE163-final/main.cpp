
#include "Shader.hpp"
#include "Geometry.hpp"
#include "Control.hpp"

#define MAINPROGRAM
#include "PointLight.hpp"
#include "DirectLight.hpp"
#include "ReadFile.hpp"
#include "variables.h"

#include "math.hpp"

using namespace std;

//initialize the window size
int width = 720;
int height = 480;
int windowX, windowY;
float zNear = 0.1f;
float zFar = 100.0f;
float fovy = 75.0f;

//initialize camera position
vec3 eye = EYE;
vec3 up = UP;
vec3 center = CENTER;

//initialize mvp
mat4 model, view, projection, modelView;

//initialize key press rotation amount
float rotateAmount = 1 / PI;

//initialize mouse setting
float moveSpeed = 0.05f, mouseSpeed = 0.1f;
float limitedFPS = 1.0f / 60.0f;

//initialize the programID mvpPos
GLuint mainProgram;
GLuint modelViewPos, projectionPos;
GLuint ambientPosition, diffusePosition, specularPosition, shininessPosition;
GLuint lightAmountPosition, lightPosPosition, lightColorPosition;
vector<vec4> lightPos;
vector<vec3> lightColor;

GLuint depthProgram;
GLuint depthMapPosition, depthMap, depthMapFBO;
GLuint lightProjectionViewPosition, lightModelPosition;

Shader * mainShader;

void printMat4(mat4 & m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << m[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

void init() {
    
    windowX = glutGet((GLenum)GLUT_WINDOW_X);
    windowY = glutGet((GLenum)GLUT_WINDOW_Y) + 46;
    
    glutSetCursor(GLUT_CURSOR_NONE);
    
    //compute the camera view
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float) width / (float) height, zNear, zFar);
    
    mainShader = new Shader("triangles.vert.glsl", "triangles.frag.glsl");
    mainShader->use();
}

void initShadowMap() {
    
}

void displayDepthMap() {
    
}

void displayMainProgram() {
    
    // reset viewport
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mainShader->use();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    
    // Update Matrices
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear, zFar);
    
    // Pass the projection to the shader
    mainShader->set("projection", projection);
    
    // Pass the lights to the shader
    mainShader->set("lightAmount", (int) lights.size());
    for (int i = 0; i < lights.size(); i++) {
        PointLight * ptlgt = static_cast<PointLight *>(lights[i]);
        DirectLight * dirlgt = static_cast<DirectLight *>(lights[i]);
        if (ptlgt != nullptr) {
            vec3 pos = ptlgt->pos;
            lightPos.push_back(vec4(pos, 1));
        }
        else if (dirlgt != nullptr) {
            vec3 dir = dirlgt->dir;
            lightPos.push_back(vec4(dir, 0));
        }
        lightColor.push_back(lights[i]->color);
    }
    mainShader->set("lightPositions", lightPos);
    mainShader->set("lightColors", lightColor);
    
    // Pass objects to the shader
    for (int i = 0; i < objects.size(); i++) {
        
        // Setup mvp
        mainShader->set("modelView", view * objects[i]->transf);
        mainShader->set("ambient", objects[i]->ambient);
        mainShader->set("diffuse", objects[i]->diffuse);
        mainShader->set("specular", objects[i]->specular);
        mainShader->set("shininess", objects[i]->shininess);
        
        // Display the object
        displayObject(objects[i]);
    }
    
    glFlush();
    glutSwapBuffers();
}

void display() {
    displayDepthMap();
    displayMainProgram();
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
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("final-project");
    
    readfile(argv[1]);
    genBuffers();
    initAllMeshes();
    initShadowMap();
    init();
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrowKey);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseRotate);
    glutMotionFunc(mouseRotate);
    
    glutMainLoop();
}
