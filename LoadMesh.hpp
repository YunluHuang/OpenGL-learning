//
//  LoadMesh.hpp
//  CSE163-final
//
//  Created by Ah Huang on 6/2/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef LoadMesh_hpp
#define LoadMesh_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#include <glm/glm.hpp>

typedef glm::vec3 vec3;

typedef struct {
    unsigned int meshID;
    const char *fileName;
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<GLuint> indices;
} Mesh;

void loadMesh(Mesh *mesh);

#endif /* LoadMesh_hpp */
