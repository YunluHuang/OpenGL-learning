//
//  Geometry.hpp
//  openGL_tutorial_1
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include <stdio.h>
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#include <glm/glm.hpp>

#include "variables.h"

#include <vector>
#include <cstdio>
#include <iostream>

//typedef glm::vec3 vec3;
//
//typedef struct {
//    unsigned int meshID;
//    const char *fileName;
//    std::vector<vec3> vertices;
//    std::vector<vec3> normals;
//    std::vector<GLuint> indices;
//} Mesh;

//Vertex attribute: poisition id = 0
enum Attrib_IDs {vPosition = 0};

void genBuffers();
void initMesh(Mesh *mesh);
//void parseFile(Mesh *mesh);
//void loadObj(Mesh *mesh);
void bindBuffers(Mesh *mesh);
void displayMesh(Mesh *mesh);

#endif /* Geometry_hpp */
