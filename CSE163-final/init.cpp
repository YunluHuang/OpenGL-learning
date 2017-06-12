//
//  init.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/5/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include "math.hpp"

#include "variables.h"

#include "DirectLight.hpp"
#include "PointLight.hpp"
#include "Skybox.hpp"
#include "Shader.hpp"
#include "Object.hpp"

using namespace std;

float zNear = 0.1f;
float zFar = 100.0f;
float fovy = 75.0f;

vec3 eye = EYE;
vec3 up = UP;
vec3 center = CENTER;

Shader * mainShader;
Shader * depthShader;
Shader * cubeDepthShader;
Shader * quadShader;
Shader * skyboxShader;

mat4 view;
mat4 projection;

int windowX;
int windowY;

std::vector<PointLight *> ptlgts;
std::vector<DirectLight *> dirlgts;

std::vector<GLuint> depthMapFBOs, depthMaps;
mat4 lightProjection;
std::vector<mat4> lightSpaceMatrices;

Skybox * skybox = nullptr;

mat4 fixedView, fixedProjection;

void genBuffers();
void initAllMeshes();
void displayObject(Object * object);

void initMainShader() {
    
    windowX = glutGet((GLenum)GLUT_WINDOW_X);
    windowY = glutGet((GLenum)GLUT_WINDOW_Y) + 46;
    
    glutSetCursor(GLUT_CURSOR_NONE);
    
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(glm::radians(fovy), (float) width / (float) height, zNear, zFar);
    
    mainShader = new Shader("triangles.vert.glsl", "triangles.frag.glsl");
}

void initDumpMaps() {
    
    // Initiate Dump Direct Light Map
    glGenFramebuffers(1, &dumpDirlgtMapFBO);
    glGenTextures(1, &dumpDirlgtMap);
    glBindTexture(GL_TEXTURE_2D, dumpDirlgtMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glBindFramebuffer(GL_FRAMEBUFFER, dumpDirlgtMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dumpDirlgtMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Initiate Dump Point Light Map
    glGenFramebuffers(1, &dumpPtlgtMapFBO);
    glGenTextures(1, &dumpPtlgtMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, dumpPtlgtMap);
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glBindFramebuffer(GL_FRAMEBUFFER, dumpPtlgtMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dumpPtlgtMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void initShadowMap() {
    depthShader = new Shader("depth.vert.glsl", "depth.frag.glsl");
    cubeDepthShader = new Shader("cubeDepth.vert.glsl", "cubeDepth.frag.glsl", "cubeDepth.geo.glsl");
    initDumpMaps();
}

void initDebugQuad() {
    quadShader = new Shader("debugQuad.vert.glsl", "debugQuad.frag.glsl");
}

void initSkybox() {
    skyboxShader = new Shader("skybox.vert.glsl", "skybox.frag.glsl");
}

void init() {
    genBuffers();
    initMainShader();
    initAllMeshes();
    initShadowMap();
    initSkybox();
    
    cout << objects.size() << " objects, " << ptlgts.size() << " point lights, " << dirlgts.size() << " direct lights." << endl;
}
