//
//  init.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/5/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
Shader * mainEnvShader;
Shader * depthShader;
Shader * cubeDepthShader;
Shader * quadShader;
Shader * skyboxShader;
Shader * irradianceShader;

Shader * shaderGeometryPass;
Shader * shaderLightingPass;
Shader * shaderSSAO;
Shader * shaderSSAOBlur;

mat4 view;
mat4 projection;

int windowX;
int windowY;

std::vector<PointLight *> ptlgts;
std::vector<DirectLight *> dirlgts;

std::vector<GLuint> depthMapFBOs, depthMaps;
mat4 lightProjection;
std::vector<mat4> lightSpaceMatrices;

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
    
    mainShader = new Shader("basic.vert.glsl", "main.frag.glsl");
    mainEnvShader = new Shader("basic.vert.glsl", "mainEnv.frag.glsl");
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

GLuint gBuffer, gPosition, gNormal, gAlbedo, rboDepth;
GLuint ssaoFBO, ssaoBlurFBO;
GLuint ssaoColorBuffer, ssaoColorBufferBlur;
GLuint noiseTexture;
std::vector<glm::vec3> ssaoKernel;

void initSSAO() {
    shaderGeometryPass = new Shader("ssaoGeometry.vert.glsl", "ssaoGeometry.frag.glsl");
    shaderLightingPass = new Shader("ssao.vert.glsl", "ssaoLighting.frag.glsl");
    shaderSSAO = new Shader("ssao.vert.glsl", "ssao.frag.glsl");
    shaderSSAOBlur = new Shader("ssao.vert.glsl", "ssaoBlur.frag.glsl");
    
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    
    {
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
        // normal color buffer
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
        // color + specular color buffer
        glGenTextures(1, &gAlbedo);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);
        // create and attach depth buffer (renderbuffer)
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    {
        glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        
        // SSAO color buffer
        glGenTextures(1, &ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;
        
        // and blur stage
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
        glGenTextures(1, &ssaoColorBufferBlur);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    {
        std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
        std::default_random_engine generator;
        for (unsigned int i = 0; i < 100; ++i) {
            glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            float scale = float(i) / 100.0f;
            
            // scale samples s.t. they're more aligned to center of kernel
            scale = 0.1f * (1.0 - scale * scale) + 1.0f * scale * scale;
            sample *= scale;
            ssaoKernel.push_back(sample);
        }
        
        std::vector<glm::vec3> ssaoNoise;
        for (unsigned int i = 0; i < 16; i++) {
            glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
            ssaoNoise.push_back(noise);
        }
        glGenTextures(1, &noiseTexture);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    
    {
        shaderLightingPass->use();
        shaderLightingPass->set("gPosition", 0);
        shaderLightingPass->set("gNormal", 1);
        shaderLightingPass->set("gAlbedo", 2);
        shaderLightingPass->set("ssao", 3);
        shaderSSAO->use();
        shaderSSAO->set("gPosition", 0);
        shaderSSAO->set("gNormal", 1);
        shaderSSAO->set("texNoise", 2);
        shaderSSAOBlur->use();
        shaderSSAOBlur->set("ssaoInput", 0);
    }
}

void initQuadShader() {
    quadShader = new Shader("debugQuad.vert.glsl", "debugQuad.frag.glsl");
}

void init() {
    genBuffers();
    initMainShader();
    initAllMeshes();
    initShadowMap();
    initSkybox();
    
    initQuadShader();
    
    initSSAO();
    
    cout << objects.size() << " objects, " << ptlgts.size() << " point lights, " << dirlgts.size() << " direct lights." << endl;
}
