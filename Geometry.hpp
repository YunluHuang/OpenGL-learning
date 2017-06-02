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

#include <vector>

typedef glm::vec3 vec3;

//Triangles take VAO[0], total VAOs = 1
enum VAO_IDs {Triangles, NumObjs};
//Vertex attribute: poisition id = 0
enum Attrib_IDs {vPosition = 0};

extern std::vector<vec3> vertices;
extern std::vector<GLuint> indices;

void initGeometry();
void loadObj();
void bindBuffer();
void displayGeometry();

#endif /* Geometry_hpp */
