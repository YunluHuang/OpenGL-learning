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
    glGenBuffers(size, &NBOs[0]);
    glGenBuffers(size, &EBOs[0]);
}

void initAllMeshes() {
    for (auto it = loadedMeshes.begin(); it != loadedMeshes.end(); it++) {
        initMesh(it->second);
    }
}

void initMesh(Mesh * mesh) {
    
    
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec3> indices;
    
    for (int i = 0; i < mesh->indices.size(); i += 3) {
        
        int v1, v2, v3, n1, n2, n3;
        v1 = mesh->indices[i], v2 = mesh->indices[i + 1], v3 = mesh->indices[i + 2];
//        n1 = mesh->normalIndicies[i];
    }
    
    
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

void displayObject(Object * object) {
    
    glBindVertexArray(VAOs[object->mesh->id]);
    
    glDrawElements(GL_TRIANGLES, (int) object->mesh->indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}






































