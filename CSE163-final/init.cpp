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
Shader * skyboxShader;

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

Skybox * skybox;

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

void initShadowMap() {
    depthShader = new Shader("depth.vert.glsl", "depth.frag.glsl");
}

void initSkybox() {
    skyboxShader = new Shader("skybox.vert.glsl", "skybox.frag.glsl");
    vector<const char *> cubeFaces = {
        "powderpeak_rt.tga",
        "powderpeak_lf.tga",
        "powderpeak_up.tga",
        "powderpeak_dn.tga",
        "powderpeak_bk.tga",
        "powderpeak_ft.tga"
    };
    skybox = new Skybox(cubeFaces);
    skybox->genCube(50);
}

void init() {
    genBuffers();
    initMainShader();
    initAllMeshes();
    initShadowMap();
    initSkybox();
    
    cout << "object: " << objects.size() << ", light: " << lights.size() << "." << endl;
    cout << loadedMeshes.size() << " meshes loaded." << endl;
}
