
#include "LoadShader.hpp"
#include "ShadowMap.hpp"
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
int width = 512;
int height = 512;
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
float moveSpeed = 0.1f, mouseSpeed = 0.1f;
float limitedFPS = 1.0f / 60.0f;

//initialize the programID mvpPos
GLuint mainProgram, depthProgram;
GLuint modelViewPos, projectionPos;
GLuint ambientPosition, diffusePosition, specularPosition, shininessPosition;
GLuint lightAmountPosition, lightPosPosition, lightColorPosition;
vector<GLfloat> lightPos, lightColor;

GLuint depthMVPPos, depthMapPos, DepthBiasID, ShadowMapID;
GLuint frameBuffer;
GLuint depthTexture;


//void printMat4(mat4 & m) {
//    for (int i = 0; i < 4; i++) {
//        for (int j = 0; j < 4; j++) {
//            std::cout << m[j][i] << " ";
//        }
//        std::cout << std::endl;
//    }
//}

void init() {
    
    windowX = glutGet((GLenum)GLUT_WINDOW_X);
    windowY = glutGet((GLenum)GLUT_WINDOW_Y) + 46;
    
    glutSetCursor(GLUT_CURSOR_NONE);
    
    //compute the camera view
    model = mat4(1.0f);
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float) width / (float) height, zNear, zFar);
    
    ShaderInfo mainShaders[] = {
        {GL_VERTEX_SHADER, "triangles.vert.glsl"},
        {GL_FRAGMENT_SHADER, "triangles.frag.glsl"},
        {GL_NONE, NULL}
    };
    
    mainProgram = loadShaders(mainShaders);
//    glUseProgram(mainProgram);
    
    // Matrices position
    projectionPos = glGetUniformLocation(mainProgram, "projection");
    modelViewPos = glGetUniformLocation(mainProgram, "modelView");
    
    // Object color related shader param
    ambientPosition = glGetUniformLocation(mainProgram, "ambient");
    specularPosition = glGetUniformLocation(mainProgram, "specular");
    diffusePosition = glGetUniformLocation(mainProgram, "diffuse");
    shininessPosition = glGetUniformLocation(mainProgram, "shininess");
    
    // Light related shader param
    lightAmountPosition = glGetUniformLocation(mainProgram, "lightAmount");
    lightPosPosition = glGetUniformLocation(mainProgram, "lightPositions");
    lightColorPosition = glGetUniformLocation(mainProgram, "lightColors");
    
    // texture shader param
    depthMapPos = glGetUniformLocation(mainProgram, "depthMap");
    
//    initAllMeshes();
}

void display() {
    
//    glBindVertexArray(VAOs[0]);
    
    //render to texture
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(depthProgram);
    
    
    glm::vec3 lightInvDir = glm::vec3(0.5f,2,2);
    
    // Compute the MVP matrix from the light's point of view
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
//    glm::mat4 depthModelMatrix = glm::mat4(1.0);
//    glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
    
    for (int i = 0; i < objects.size(); i++) {
        mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * objects[i]->transf;
        glUniformMatrix4fv(depthMVPPos, 1, GL_FALSE, &depthMVP[0][0]);
        // Display the object
        displayObject(objects[i]);
    }
    
    
    // render to the scene
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    
    glUseProgram(mainProgram);
    
    glm::mat4 biasMatrix(
                         0.5, 0.0, 0.0, 0.0,
                         0.0, 0.5, 0.0, 0.0,
                         0.0, 0.0, 0.5, 0.0,
                         0.5, 0.5, 0.5, 1.0
                         );
    
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(ShadowMapID, 1);
    
    
    
    //render noraml scene
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    // Update Matrices
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear, zFar);
    
    // Pass the projection to the shader
    glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
    
    // Pass the lights to the shader
    glUniform1i(lightAmountPosition, (int) lights.size());
    for (int i = 0; i < lights.size(); i++) {
        PointLight * ptlgt = static_cast<PointLight *>(lights[i]);
        DirectLight * dirlgt = static_cast<DirectLight *>(lights[i]);
        if (ptlgt != nullptr) {
            vec3 pos = ptlgt->pos;
            lightPos.push_back(pos[0]);
            lightPos.push_back(pos[1]);
            lightPos.push_back(pos[2]);
            lightPos.push_back(1);
        }
        else if (dirlgt != nullptr) {
            vec3 dir = dirlgt->dir;
            lightPos.push_back(dir[0]);
            lightPos.push_back(dir[1]);
            lightPos.push_back(dir[2]);
            lightPos.push_back(0);
        }
        lightColor.push_back(lights[i]->color[0]);
        lightColor.push_back(lights[i]->color[1]);
        lightColor.push_back(lights[i]->color[2]);
    }
    glUniform4fv(lightPosPosition, (int) lights.size(), &lightPos[0]);
    glUniform3fv(lightColorPosition, (int) lights.size(), &lightColor[0]);
    
    // pass depthMap to the shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glUniform1i(depthMapPos, 0);
    
    // Pass objects to the shader
    for (int i = 0; i < objects.size(); i++) {
        
        /////////////////////////////////
        mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * objects[i]->transf;
        glm::mat4 depthBiasMVP = biasMatrix * depthMVP;
        glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);
        
        
        
        
        
        
        
        //////////////////////////////////
        
        
        
        
        // Setup mvp
        modelView = view * objects[i]->transf;
        glUniformMatrix4fv(modelViewPos, 1, GL_FALSE, &modelView[0][0]);
        
        // Pass color to the shader
        glUniform3f(ambientPosition, objects[i]->ambient[0], objects[i]->ambient[1], objects[i]->ambient[2]);
        glUniform3f(diffusePosition, objects[i]->diffuse[0], objects[i]->diffuse[1], objects[i]->diffuse[2]);
        glUniform3f(specularPosition, objects[i]->specular[0], objects[i]->specular[1], objects[i]->specular[2]);
        glUniform1f(shininessPosition, objects[i]->shininess);
        
        // Display the object
        displayObject(objects[i]);
    }
    
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
    genBuffers();
    initAllMeshes();
    
    //renderShadow();
    ShaderInfo depthShaders[] = {
        {GL_VERTEX_SHADER, "depth.vert.glsl"},
        {GL_FRAGMENT_SHADER, "depth.frag.glsl"},
        {GL_NONE, NULL}
    };
    depthProgram = loadShaders(depthShaders);
    
    depthMVPPos = glGetUniformLocation(depthProgram, "depthMVP");
    
    frameBuffer = 0;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
    
    glDrawBuffer(GL_NONE);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cerr << "bind frame buffer error" << endl;
    
    DepthBiasID = glGetUniformLocation(mainProgram, "DepthBiasMVP");
    ShadowMapID = glGetUniformLocation(mainProgram, "shadowMap");
    
    
    // render the scene
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
