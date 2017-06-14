//
//  variables.h
//  CSE163-final
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef variables_h
#define variables_h

#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <stack>
#include <vector>
#include <map>

#include "math.hpp"
#include "PointLight.hpp"
#include "DirectLight.hpp"
#include "Mesh.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Skybox.hpp"

// ------------ Window --------------

extern int width, height;
extern int windowX, windowY;

// ------------ Control --------------

extern float rotateAmount;
extern float moveSpeed;
extern float mouseSpeed;
extern float limitedFPS;
extern float yaw, pitch;
extern bool activeKey[128];
extern int mode;
extern const int NORMAL_MODE;
extern const int SSAO_MODE;
extern const int DEBUG_MODE;
extern bool animate;

// ------------ Camera --------------

extern vec3 eye;
extern vec3 up;
extern vec3 center;
extern float fovy, zNear, zFar;
extern mat4 view, projection, modelView;

extern bool useMotionBlur;
extern int motionBlur;

// ------------ Shader --------------

extern Shader * mainShader;
extern Shader * mainEnvShader;
extern Shader * depthShader;
extern Shader * cubeDepthShader;
extern Shader * quadShader;
extern Shader * skyboxShader;

extern Shader * shaderGeometryPass;
extern Shader * shaderLightingPass;
extern Shader * shaderSSAO;
extern Shader * shaderSSAOBlur;

// ------------ Skybox --------------

extern Skybox * skybox;
extern mat4 fixedView, fixedProjection;

// ------------ Skybox --------------

extern GLuint gBuffer, gPosition, gNormal, gAlbedo;
extern GLuint rboDepth;
extern GLuint ssaoFBO, ssaoBlurFBO;
extern GLuint ssaoColorBuffer, ssaoColorBufferBlur;
extern std::vector<glm::vec3> ssaoKernel;
extern GLuint noiseTexture;

// ------------ Objects & Buffers --------------

extern std::vector<GLuint> VAOs;
extern std::vector<GLuint> VBOs;
extern std::vector<GLuint> NBOs;
extern std::vector<GLuint> EBOs;

extern std::stack<mat4> transfStack;
extern std::vector<Object *> objects;
extern std::map<std::string, Mesh *> loadedMeshes;

extern vec3 ambient;
extern vec3 diffuse;
extern vec3 specular;
extern GLfloat shininess;

// ------------ Lights & ShadowMaps --------------

extern std::vector<PointLight *> ptlgts;
extern std::vector<DirectLight *> dirlgts;

extern GLuint dumpDirlgtMap;
extern GLuint dumpDirlgtMapFBO;
extern GLuint dumpPtlgtMap;
extern GLuint dumpPtlgtMapFBO;

#endif
