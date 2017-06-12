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

void displayDirlgtDepthMap() {
    
    glCullFace(GL_FRONT);
    
    depthShader->use();
    
    for (int i = 0; i < dirlgts.size(); i++) {
        
        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, dirlgts[i]->depthMapFBO);
        
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        depthShader->set("lightProjectionView", dirlgts[i]->getLightSpace());
        
        for (int j = 0; j < objects.size(); j++) {
            depthShader->set("lightModel", objects[j]->transf);
            displayObject(objects[j]);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    glCullFace(GL_BACK);
}

void displayPtlgtDepthMap() {
    
    glCullFace(GL_FRONT);
    
    cubeDepthShader->use();
    
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);
    
    for (int i = 0; i < ptlgts.size(); i++) {
        
        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, ptlgts[i]->depthMapFBO);
        
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        vec3 lightPos = ptlgts[i]->pos;

        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(projection * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(projection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
        shadowTransforms.push_back(projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
        shadowTransforms.push_back(projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        
        cubeDepthShader->set("lightPos", lightPos);
        
        for (GLuint j = 0; j < 6; j++) {
            cubeDepthShader->set("shadowMatrices", j, shadowTransforms[j]);
        }
        
        for (int j = 0; j < objects.size(); j++) {
            cubeDepthShader->set("model", objects[j]->transf);
            displayObject(objects[j]);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    glCullFace(GL_BACK);
}

void displayDepthMap() {
    displayDirlgtDepthMap();
    displayPtlgtDepthMap();
}

void displayMainProgram() {
    
    // reset viewport
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mainShader->use();
    
    // Update Matrices
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear, zFar);
    mainShader->set("view", view);
    mainShader->set("projection", projection);
    
    mainShader->set("dirlgtAmount", (int) dirlgts.size());
    for (int i = 0; i < dirlgts.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, dirlgts[i]->depthMap);
        mainShader->set(("dirlgtMaps[" + to_string(i) + "]").c_str(), i);
        mainShader->set(("dirlgtMatrices[" + to_string(i) + "]").c_str(), biasMatrix * dirlgts[i]->getLightSpace());
        mainShader->set(("dirlgtDirections[" + to_string(i) + "]").c_str(), dirlgts[i]->dir);
        mainShader->set(("dirlgtColors[" + to_string(i) + "]").c_str(), dirlgts[i]->color);
    }
    
    mainShader->set("ptlgtAmount", (int) ptlgts.size());
    for (int i = 0; i < ptlgts.size(); i++) {
        int tid = dirlgts.size() + i;
        glActiveTexture(GL_TEXTURE0 + tid);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ptlgts[i]->depthMap);
        mainShader->set(("ptlgtMaps[" + to_string(i) + "]").c_str(), tid);
        mainShader->set("ptlgtPositions", i, ptlgts[i]->pos);
        mainShader->set("ptlgtColors", i, ptlgts[i]->color);
    }
    
    // Pass objects to the shader
    for (int i = 0; i < objects.size(); i++) {
        
        // Setup mvp
        mainShader->set("model", objects[i]->transf);
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
