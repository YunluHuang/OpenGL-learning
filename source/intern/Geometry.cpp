//
//  Geometry.cpp
//  openGL_tutorial_1
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "Geometry.hpp"

using namespace std;

void genBuffers() {
    int size = (int) loadedMeshes.size();
    glGenVertexArrays(size, &VAOs[0]);
    glGenBuffers(size, &VBOs[0]);
    glGenBuffers(size, &EBOs[0]);
}

void initAllMeshes() {
    for (auto it = loadedMeshes.begin(); it != loadedMeshes.end(); it++) {
        initMesh(it->second);
    }
}

void initMesh(Mesh * mesh) {
    
    //bind VAOs and buffers
    glBindVertexArray(VAOs[mesh->id]);
    
    //bind array buffer for vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[mesh->id]);
    
    //copy data from vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh->vertices.size(), &(mesh->vertices)[0], GL_STATIC_DRAW);
    
    //bind buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[mesh->id]);
    
    //copy data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->indices.size(), &(mesh->indices)[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void displayObject(Object * object) {
    
    
    glBindVertexArray(VAOs[object->mesh->id]);
    glDrawElements(GL_TRIANGLES, (int) object->mesh->indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}






































