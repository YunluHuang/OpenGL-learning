//
//  Geometry.hpp
//  openGL_tutorial_1
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp
//include OpenGL and glut
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>

//force glm to use radians
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
//include glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;

#define PI = 3.14159265f

using namespace std;

vector<vec3> meshVertices;
vector<vec3> meshNormals;
vector<unsigned int> meshIndices;

void initMesh();
void refreshMesh();
void solidMesh(float size);


#endif /* Geometry_hpp */
