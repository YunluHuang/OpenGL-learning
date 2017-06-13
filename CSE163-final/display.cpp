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

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
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

unsigned int smQuadVAO = 0;
unsigned int smQuadVBO;
void renderSmallQuad() {
    if (smQuadVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  -0.5f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f,  -0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
//        float quadVertices[] = {
//            // positions        // texture Coords
//            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
//            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
//            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//        };
        // setup plane VAO
        glGenVertexArrays(1, &smQuadVAO);
        glGenBuffers(1, &smQuadVBO);
        glBindVertexArray(smQuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, smQuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(smQuadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void displaySSAO() {
    
    {
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderGeometryPass->use();
        shaderGeometryPass->set("projection", projection);
        shaderGeometryPass->set("view", view);
        
        // objects
        shaderGeometryPass->set("invertedNormals", 0);
        for (int i = 0; i < objects.size(); i++) {
            shaderGeometryPass->set("model", objects[i]->transf);
            displayObject(objects[i]);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    {
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderSSAO->use();
        // Send kernel + rotation
        for (unsigned int i = 0; i < 100; ++i)
            shaderSSAO->set(("samples[" + std::to_string(i) + "]").c_str(), ssaoKernel[i]);
        shaderSSAO->set("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    {
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderSSAOBlur->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

// Bias Matrix for shadow map
mat4 biasMatrix(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);

void displayDirlgtDepthMap() {
    
    glCullFace(GL_FRONT);
    
    depthShader->use();
    
    // First setup the dump map
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, dumpDirlgtMapFBO);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Setup the projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 25.0f);
    
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
    
    Shader * shader = skybox ? mainEnvShader : mainShader;
    
    shader->use();
    
    // Setup screen variable
    shader->set("viewPos", eye);
    shader->set("width", width);
    shader->set("height", height);
    
    // Update Matrices
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float)width / (float)height, zNear, zFar);
    shader->set("view", view);
    shader->set("projection", projection);
    
    // Initiate Dump Dir Map
    int dumpDirlgtMapPos = (int) dirlgts.size() + (int) ptlgts.size();
    glActiveTexture(GL_TEXTURE0 + dumpDirlgtMapPos);
    glBindTexture(GL_TEXTURE_2D, dumpDirlgtMap);
    
    // Initiate Dump Pt Map
    int dumpPtlgtMapPos = dumpDirlgtMapPos + 1;
    glActiveTexture(GL_TEXTURE0 + dumpPtlgtMapPos);
    glBindTexture(GL_TEXTURE_CUBE_MAP, dumpPtlgtMap);
    
    // Pass Direct Lights to the map
    shader->set("dirlgtAmount", (int) dirlgts.size());
    for (int i = 0; i < 5; i++) {
        if (i < dirlgts.size()) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, dirlgts[i]->depthMap);
            shader->set("dirlgtMaps", i, i);
            shader->set("dirlgtMatrices", i, biasMatrix * dirlgts[i]->getLightSpace());
            shader->set("dirlgtDirections", i, dirlgts[i]->dir);
            shader->set("dirlgtColors", i, dirlgts[i]->color);
        }
        else {
            shader->set("dirlgtMaps", i, dumpDirlgtMapPos);
            shader->set("dirlgtMatrices", i, mat4(1.0f));
            shader->set("dirlgtDirections", i, vec3(1.0f));
            shader->set("dirlgtColors", i, vec3(1.0f));
        }
    }
    
    // Pass the point lights to the map
    shader->set("ptlgtAmount", (int) ptlgts.size());
    for (int i = 0; i < 5; i++) {
        if (i < ptlgts.size()) {
            int tid = (int) dirlgts.size() + i;
            glActiveTexture(GL_TEXTURE0 + tid);
            glBindTexture(GL_TEXTURE_CUBE_MAP, ptlgts[i]->depthMap);
            shader->set("ptlgtMaps", i, tid);
            shader->set("ptlgtPositions", i, ptlgts[i]->pos);
            shader->set("ptlgtColors", i, ptlgts[i]->color);
        }
        else {
            shader->set("ptlgtMaps", i, dumpPtlgtMapPos);
            shader->set("ptlgtPositions", i, vec3(1.0f));
            shader->set("ptlgtColors", i, vec3(1.0f));
        }
    }
    
    // Setup environment map
    int envMapPos = dumpPtlgtMapPos + 1;
    glActiveTexture(GL_TEXTURE0 + envMapPos);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubeMapID);
    shader->set("envMap", envMapPos);
    
    // Setup irradiance map
    int irrMapPos = envMapPos + 1;
    glActiveTexture(GL_TEXTURE0 + irrMapPos);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->irradianceMapID);
    shader->set("irrMap", irrMapPos);
    
    // Setup SSAO map
    int ssaoTexturePos = irrMapPos + 1;
    glActiveTexture(GL_TEXTURE0 + ssaoTexturePos);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    shader->set("ssao", ssaoTexturePos);
    
    // Pass objects to the shader
    for (int i = 0; i < objects.size(); i++) {
        
        // Setup mvp
        shader->set("model", objects[i]->transf);
        shader->set("ambient", objects[i]->ambient);
        shader->set("diffuse", objects[i]->diffuse);
        shader->set("specular", objects[i]->specular);
        shader->set("shininess", objects[i]->shininess);
        
        // Display the object
        displayObject(objects[i]);
    }
}

void displaySkyBox() {
    if (skybox) {
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
}

void displayQuad(GLuint texture) {
    quadShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    renderSmallQuad();
}

void processAnimation() {
    for (int i = 0; i < ptlgts.size(); i++) {
        ptlgts[i]->pos = vec3(glm::rotate(mat4(1.0f), glm::radians(1.0f), vec3(0, 1, 0)) * vec4(ptlgts[i]->pos, 1.0f));
    }
}

void display() {
    
    processAnimation();
    
    // First process keyboard input
    processKeyboard();
    
    displaySSAO();
    
    // Then render the depth map
    displayDepthMap();
    
    // render the skybox
    // TODO: this need to be rendered last in further development
    displaySkyBox();
    
    // Then execute the main render program
    displayMainProgram();
    
    // Debug
    displayQuad(ssaoColorBufferBlur);
    
    // Flush the viewport and swap the buffer
    glFlush();
    glutSwapBuffers();
}

void idle() {
    display();
}
