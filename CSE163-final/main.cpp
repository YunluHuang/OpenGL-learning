
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
vector<vec4> lightPos;
vector<vec3> lightColor;

void printMat4(mat4 & m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << m[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad() {
    if (quadVAO == 0) {
        
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  -0.3f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.3f,  -0.3f, 0.0f, 1.0f, 1.0f,
            -0.3f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
    
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

Shader * mainShader;

void initMainShader() {
    
    windowX = glutGet((GLenum)GLUT_WINDOW_X);
    windowY = glutGet((GLenum)GLUT_WINDOW_Y) + 46;
    
    glutSetCursor(GLUT_CURSOR_NONE);
    
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float) width / (float) height, zNear, zFar);
    
    mainShader = new Shader("triangles.vert.glsl", "triangles.frag.glsl");
}

Shader * depthShader;
GLuint depthMapFBO, depthMap;

void initShadowMap() {
    
    depthShader = new Shader("depth.vert.glsl", "depth.frag.glsl");
    
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Shader * quadShader;
void initQuad() {
    quadShader = new Shader("debugQuad.vert.glsl", "debugQuad.frag.glsl");
}

void init() {
    genBuffers();
    initMainShader();
    initAllMeshes();
    initShadowMap();
    initQuad();
    
    mainShader->use();
    mainShader->set("depthMap", 0);
    
    quadShader->use();
    quadShader->set("depthMap", 0);
}

glm::mat4 lightProjection, lightView;
glm::mat4 lightSpaceMatrix;

void displayDepthMap() {
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::vec3 lightPos(-4.0f, 2.0f, 3.0f);
    
    float near_plane = 1.0f, far_plane = 7.5f;
    
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    
    // render scene from light's point of view
    depthShader->use();
    depthShader->set("lightProjectionView", lightSpaceMatrix);
    
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Pass objects to the shader
    for (int i = 0; i < objects.size(); i++) {
        
        // Setup mvp
        depthShader->set("lightModel", objects[i]->transf);
        
        // Display the object
        displayObject(objects[i]);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    
    glm::mat4 biasMatrix(
                         0.5, 0.0, 0.0, 0.0,
                         0.0, 0.5, 0.0, 0.0,
                         0.0, 0.0, 0.5, 0.0,
                         0.5, 0.5, 0.5, 1.0
                         );
    glm::mat4 depthBiasVP = biasMatrix * lightSpaceMatrix;
    mainShader->set("lightSpaceMatrix", depthBiasVP);
    
    // Pass objects to the shader
    for (int i = 0; i < objects.size(); i++) {
        
        // Setup mvp
        mainShader->set("model", objects[i]->transf);
        mainShader->set("view", view);
        mainShader->set("ambient", objects[i]->ambient);
        mainShader->set("diffuse", objects[i]->diffuse);
        mainShader->set("specular", objects[i]->specular);
        mainShader->set("shininess", objects[i]->shininess);
        
        // Display the object
        displayObject(objects[i]);
    }
}

void displayQuad() {
    
    quadShader->use();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    renderQuad();
}

void display() {
    displayDepthMap();
    displayMainProgram();
    displayQuad();
    
    glFlush();
    glutSwapBuffers();
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
