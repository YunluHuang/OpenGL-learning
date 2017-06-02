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

enum Primitives {Triangles};
//Vertex attribute: poisition id = 0
enum Attrib_IDs {vPosition = 0};

void genBuffers();
void initMesh(Mesh *mesh);
void initPrimitive(Primitives primitive);
void displayMesh(Mesh *mesh);
void displayPrimitive(Primitives primitive);

#endif /* Geometry_hpp */
