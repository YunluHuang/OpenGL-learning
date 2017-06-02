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

#include "LoadMesh.hpp"

#include <vector>

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

extern int width, height;

extern glm::vec3 eye;
extern glm::vec3 up;
extern glm::vec3 center;
extern float fovy, zNear, zFar;
extern float rotateAmount;

extern GLuint mvpPos;

extern int numTypes;

extern std::vector<GLuint> VAOs;
extern std::vector<GLuint> VBOs;
extern std::vector<GLuint> NBOs;
extern std::vector<GLuint> EBOs;

extern std::vector<Mesh *> Meshes;

extern unsigned int TriangleID;

//extern std::vector<std::vector<glm::vec3>> objVertices;
//extern std::vector<std::vector<glm::vec3>> objNormals;
//extern std::vector<std::vector<GLuint>> objIndices;
