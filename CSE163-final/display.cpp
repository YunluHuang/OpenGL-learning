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
    
    for (int i = 0; i < dirlgts.size(); i++) {
        
        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, dirlgts[i]->depthMapFBO);
        
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        depthShader->set("lightProjectionView", dirlgts[i]->getLightSpace());
        
        for (int i = 0; i < objects.size(); i++) {
            depthShader->set("lightModel", objects[i]->transf);
            displayObject(objects[i]);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    cubeDepthShader->use();
    
    for (int i = 0; i < ptlgts.size(); i++) {
        
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
    
    mainShader->set("dirlgtAmount", (int) dirlgts.size());
    for (int i = 0; i < dirlgts.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, dirlgts[i]->depthMap);
        mainShader->set(("dirlgtMaps[" + to_string(i) + "]").c_str(), i);
        mainShader->set(("dirlgtMatrices[" + to_string(i) + "]").c_str(), biasMatrix * dirlgts[i]->getLightSpace());
        mainShader->set(("dirlgtDirections[" + to_string(i) + "]").c_str(), dirlgts[i]->getLightPosition());
        mainShader->set(("dirlgtColors[" + to_string(i) + "]").c_str(), dirlgts[i]->color);
    }
    
    mainShader->set("ptlgtAmount", (int) ptlgts.size());
    for (int i = 0; i < ptlgts.size(); i++) {
        
    }
    
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

void display() {
    
    // First process keyboard input
    processKeyboard();
    
    // Then render the depth map
    displayDepthMap();
    
    // Then execute the main render program
    displayMainProgram();
    
    // Flush the viewport and swap the buffer
    glFlush();
    glutSwapBuffers();
}

void idle() {
    display();
}
