//
//  init.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/5/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include <stdio.h>
#include "math.hpp"

#include "variables.h"

#include "Shader.hpp"

using namespace std;

float zNear = 0.1f;
float zFar = 100.0f;
float fovy = 75.0f;

vec3 eye = EYE;
vec3 up = UP;
vec3 center = CENTER;

Shader * mainShader;
Shader * depthShader;

mat4 view;
mat4 projection;

int windowX;
int windowY;

std::vector<Light *> lights;
std::vector<vec4> lightPos;
std::vector<vec3> lightColor;

std::vector<GLuint> depthMapFBOs, depthMaps;
mat4 lightProjection;
std::vector<mat4> lightSpaceMatrices;

void genBuffers();
void initAllMeshes();

void initMainShader() {
    
    windowX = glutGet((GLenum)GLUT_WINDOW_X);
    windowY = glutGet((GLenum)GLUT_WINDOW_Y) + 46;
    
    glutSetCursor(GLUT_CURSOR_NONE);
    
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float) width / (float) height, zNear, zFar);
    
    mainShader = new Shader("triangles.vert.glsl", "triangles.frag.glsl");
}

void initShadowMap() {
    
    depthShader = new Shader("depth.vert.glsl", "depth.frag.glsl");
    
    for (int i = 0; i < lights.size(); i++) {
        depthMapFBOs.push_back(0);
        depthMaps.push_back(0);
    }
    
    glGenFramebuffers(10, &depthMapFBOs[0]);
    glGenTextures(10, &depthMaps[0]);
    
    for (int i = 0; i < lights.size(); i++) {
        
        glBindTexture(GL_TEXTURE_2D, depthMaps[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOs[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMaps[i], 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void init() {
    genBuffers();
    initMainShader();
    initAllMeshes();
    initShadowMap();
    
    mainShader->use();
    for (int i = 0; i < lights.size(); i++) {
        mainShader->set(("depthMaps[" + to_string(i) + "]").c_str(), depthMaps[i]);
    }
}
