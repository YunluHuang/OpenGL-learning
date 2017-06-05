//
//  variables.h
//  CSE163-final
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef Variables_H
#define Variables_H

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
#include "Light.hpp"
#include "Mesh.hpp"
#include "Object.hpp"
#include "Shader.hpp"

extern int width, height;
extern int windowX, windowY;

extern vec3 eye;
extern vec3 up;
extern vec3 center;
extern float fovy, zNear, zFar;
extern float rotateAmount;
extern float moveSpeed;
extern float mouseSpeed;
extern float limitedFPS;

extern mat4 view, projection, modelView;

extern Shader * mainShader;
extern Shader * depthShader;
extern std::vector<GLuint> depthMapFBOs, depthMaps;

extern mat4 lightProjection;
extern std::vector<mat4> lightSpaceMatrices;

//GLuint depthMap;
//GLuint depthMapID;

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

extern GLuint ambientPosition;
extern GLuint specularPosition;
extern GLuint diffusePosition;
extern GLuint shininessPosition;

extern std::vector<Light *> lights;

extern std::vector<vec4> lightPos;
extern std::vector<vec3> lightColor;

#endif