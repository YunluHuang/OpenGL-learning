//
//  display.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/5/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include <stdio.h>

#include "variables.h"
#include "math.hpp"

#include "PointLight.hpp"
#include "DirectLight.hpp"
#include "Skybox.hpp"

using namespace std;

// Forward declarations
void processKeyboard();
void displayObject(Object * object);

// Redeclare buffer objects
std::vector<GLuint> VAOs;
std::vector<GLuint> VBOs;
std::vector<GLuint> NBOs;
std::vector<GLuint> EBOs;

// Bias Matrix for shadow map
mat4 biasMatrix(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);

void displayDepthMap() {
    
    glCullFace(GL_FRONT);
    
    depthShader->use();
    
    for (int i = 0; i < lights.size(); i++) {
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, lights[i]->depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        lightSpaceMatrices[i] = lights[i]->getLightSpace();
        
        depthShader->set("lightProjectionView", lightSpaceMatrices[i]);
        
        // Pass objects to the shader
        for (int i = 0; i < objects.size(); i++) {
            
            // Setup mvp
            depthShader->set("lightModel", objects[i]->transf);
            
            // Display the object
            displayObject(objects[i]);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    glCullFace(GL_BACK);
}

void displayMainProgram() {
    
    // reset viewport
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mainShader->use();
    
    // Update Matrices
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear, zFar);
    
    // Pass the projection to the shader
    mainShader->set("projection", projection);
    
    // Pass the lights to the shader
    mainShader->set("lightAmount", (int) lights.size());
    for (int i = 0; i < lights.size(); i++) {
        
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, lights[i]->depthMap);
        
        mat4 depthBiasVP = biasMatrix * lights[i]->getLightSpace();
        mainShader->set(("lightSpaceMatrices[" + to_string(i) + "]").c_str(), depthBiasVP);
        mainShader->set(("depthMaps[" + to_string(i) + "]").c_str(), i);
        
        lightPos[i] = lights[i]->getLightPosition();
        lightColor[i] = lights[i]->color;
    }
    mainShader->set("lightPositions", lightPos);
    mainShader->set("lightColors", lightColor);
    
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

void displaySkyBox() {
    
    cout << "displaySkyBox" << endl;
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    skyboxShader->use();
    
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear, zFar);
    
    //pass the projection and view matrix to shader
    skyboxShader->set("projection", projection);
    skyboxShader->set("view", view);
    skyboxShader->set("skybox", 0);
    skyboxShader->set("eyePos", eye);
    
    glBindVertexArray(skybox->skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubeMapID);
   
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void display() {
    
    // First process keyboard input
    processKeyboard();
    
    // render the skybox
    // TODO: this need to be rendered last in further development
    displaySkyBox();
    
    // Then render the depth map
    //displayDepthMap();
    
    // Then execute the main render program
    //displayMainProgram();
    
    // Flush the viewport and swap the buffer
    glFlush();
    glutSwapBuffers();
}

void idle() {
    display();
}
