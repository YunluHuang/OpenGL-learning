//
//  variables.h
//  CSE163-final
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>
#include <map>

#include "math.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "Object.hpp"

extern int width, height;

extern vec3 eye;
extern vec3 up;
extern vec3 center;
extern float fovy, zNear, zFar;
extern float rotateAmount;
extern float moveSpeed, mouseSpeed, limitedFPS;

extern mat4 model, view, projection, modelView;
extern GLuint modelViewPos, projectionPos;

extern std::vector<GLuint> VAOs;
extern std::vector<GLuint> VBOs;
extern std::vector<GLuint> NBOs;
extern std::vector<GLuint> EBOs;

extern std::vector<Object *> objects;
extern std::map<std::string, Mesh *> loadedMeshes;

extern std::vector<Light *> lights;

//extern std::vector<std::vector<glm::vec3>> objVertices;
//extern std::vector<std::vector<glm::vec3>> objNormals;
//extern std::vector<std::vector<GLuint>> objIndices;
