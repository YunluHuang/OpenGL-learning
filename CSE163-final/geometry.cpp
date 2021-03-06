//
//  geometry.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/5/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef geometry_cpp
#define geometry_cpp

#include <stdio.h>

#include <vector>
#include <cstdio>
#include <iostream>

#include "variables.h"
#include "Mesh.hpp"

using namespace std;

//Vertex attribute: poisition id = 0
enum Attrib_IDs {vPosition = 0, vNormal = 1, vFace = 2};

void genBuffers() {
    int size = (int) loadedMeshes.size();
    glGenVertexArrays(size, &VAOs[0]);
    glGenBuffers(size, &VBOs[0]);
    glGenBuffers(size, &NBOs[0]);
    glGenBuffers(size, &EBOs[0]);
}

void initMesh(Mesh * mesh) {
    
    //bind buffers to VAO
    glBindVertexArray(VAOs[mesh->id]);
    
    //bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[mesh->id]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh->vertices.size(), &(mesh->vertices)[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);
    
    //bind NBO
    glBindBuffer(GL_ARRAY_BUFFER, NBOs[mesh->id]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh->normals.size(), &(mesh->normals)[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vNormal);
    
    //bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[mesh->id]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->indices.size(), &(mesh->indices)[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(vFace, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vFace);
}

void initAllMeshes() {
    for (auto it = loadedMeshes.begin(); it != loadedMeshes.end(); it++) {
        initMesh(it->second);
    }
}

void displayObject(Object * object) {
    glBindVertexArray(VAOs[object->mesh->id]);
    glDrawElements(GL_TRIANGLES, (int) object->mesh->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

#endif
