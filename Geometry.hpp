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
#include <cstdio>
#include <iostream>

#include "variables.h"
#include "Mesh.hpp"

//Vertex attribute: poisition id = 0
enum Attrib_IDs {vPosition = 0};

void genBuffers();
void initMesh(Mesh * mesh);
void displayMesh(Mesh * mesh);

#endif /* Geometry_hpp */
