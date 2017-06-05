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

#include "Geometry.hpp"
#include "PointLight.hpp"
#include "DirectLight.hpp"

using namespace std;

std::vector<GLuint> VAOs;
std::vector<GLuint> VBOs;
std::vector<GLuint> NBOs;
std::vector<GLuint> EBOs;

void displayDepthMap() {
    
    // render scene from light's point of view
    depthShader->use();
    
    lightProjection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 1.0f, 7.5f);
    
    for (int i = 0; i < lights.size(); i++) {
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOs[i]);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        vec3 lightPos;
        
        PointLight * ptlgt = static_cast<PointLight *>(lights[i]);
        DirectLight * dirlgt = static_cast<DirectLight *>(lights[i]);
        if (ptlgt) lightPos = ptlgt->pos;
        else lightPos = -dirlgt->dir;
        mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        
        lightSpaceMatrices.push_back(lightProjection * lightView);
        
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
    mat4 biasMatrix(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);
    mainShader->set("lightAmount", (int) lights.size());
    for (int i = 0; i < lights.size(); i++) {
        
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, depthMaps[i]);
        
        mat4 depthBiasVP = biasMatrix * lightSpaceMatrices[i];
        mainShader->set(("lightSpaceMatrices[" + to_string(i) + "]").c_str(), depthBiasVP);
        mainShader->set(("depthMaps[" + to_string(i) + "]").c_str(), i);
        
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
    displayDepthMap();
    displayMainProgram();
    
    glFlush();
    glutSwapBuffers();
}

void idle() {
    // display();
}
