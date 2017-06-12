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

GLuint dumpDirlgtMap;
GLuint dumpDirlgtMapFBO;
GLuint dumpPtlgtMap;
GLuint dumpPtlgtMapFBO;

// Bias Matrix for shadow map
mat4 biasMatrix(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);

void displayDirlgtDepthMap() {
    
    glCullFace(GL_FRONT);
    
    depthShader->use();
    
    // First setup the dump map
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, dumpDirlgtMapFBO);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Then render the direct light shadow map
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
    
    // First setup the dump map
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, dumpPtlgtMapFBO);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Setup the projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);
    
    // Render the point light shadow map
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
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mainShader->use();
    
    // Update Matrices
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear, zFar);
    mainShader->set("view", view);
    mainShader->set("projection", projection);
    
    // Initiate Dump Dir Map
    int dumpDirlgtMapPos = (int) dirlgts.size() + (int) ptlgts.size();
    glActiveTexture(GL_TEXTURE0 + dumpDirlgtMapPos);
    glBindTexture(GL_TEXTURE_2D, dumpDirlgtMap);
    
    // Initiate Dump Pt Map
    int dumpPtlgtMapPos = dumpDirlgtMapPos + 1;
    glActiveTexture(GL_TEXTURE0 + dumpPtlgtMapPos);
    glBindTexture(GL_TEXTURE_CUBE_MAP, dumpPtlgtMap);
    
    // Pass Direct Lights to the map
    mainShader->set("dirlgtAmount", (int) dirlgts.size());
    for (int i = 0; i < 5; i++) {
        if (i < dirlgts.size()) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, dirlgts[i]->depthMap);
            mainShader->set("dirlgtMaps", i, i);
            mainShader->set("dirlgtMatrices", i, biasMatrix * dirlgts[i]->getLightSpace());
            mainShader->set("dirlgtDirections", i, dirlgts[i]->dir);
            mainShader->set("dirlgtColors", i, dirlgts[i]->color);
        }
        else {
            mainShader->set("dirlgtMaps", i, dumpDirlgtMapPos);
            mainShader->set("dirlgtMatrices", i, mat4(1.0f));
            mainShader->set("dirlgtDirections", i, vec3(1.0f));
            mainShader->set("dirlgtColors", i, vec3(1.0f));
        }
    }
    
    // Pass the point lights to the map
    mainShader->set("ptlgtAmount", (int) ptlgts.size());
    for (int i = 0; i < 5; i++) {
        if (i < ptlgts.size()) {
            int tid = (int) dirlgts.size() + i;
            glActiveTexture(GL_TEXTURE0 + tid);
            glBindTexture(GL_TEXTURE_CUBE_MAP, ptlgts[i]->depthMap);
            mainShader->set("ptlgtMaps", i, tid);
            mainShader->set("ptlgtPositions", i, ptlgts[i]->pos);
            mainShader->set("ptlgtColors", i, ptlgts[i]->color);
        }
        else {
            mainShader->set("ptlgtMaps", i, dumpPtlgtMapPos);
            mainShader->set("ptlgtPositions", i, vec3(1.0f));
            mainShader->set("ptlgtColors", i, vec3(1.0f));
        }
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

void displaySkyBox() {
    
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    skyboxShader->use();
    
    fixedView = mat4(mat3(glm::lookAt(eye, center, up)));
    fixedProjection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear , zFar);
    
    //pass the projection and view matrix to shader
    skyboxShader->set("projection", fixedProjection);
    skyboxShader->set("view", fixedView);
    skyboxShader->set("skybox", 0);
    
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
    displayMainProgram();
    
    // Flush the viewport and swap the buffer
    glFlush();
    glutSwapBuffers();
}

void idle() {
    display();
}
