//
//  Geometry.cpp
//  openGL_tutorial_1
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "Geometry.hpp"

using namespace std;

//defined for Triangle primitive
GLfloat TriangleVertices[6][2] = {
    { -0.30, -0.30 },
    {  0.30, -0.30 },
    { -0.30,  0.30 },
    {  0.30, -0.30 },
    {  0.30,  0.30 },
    { -0.30,  0.30 }
};

void genBuffers() {
    glGenVertexArrays(numTypes, &VAOs[0]);
    glGenBuffers(numTypes, &VBOs[0]);
    glGenBuffers(numTypes, &EBOs[0]);
}

void initMesh(Mesh *mesh) {
    loadMesh(mesh);
    //bind VAOs and buffers
    glBindVertexArray(VAOs[mesh->meshID]);
    //bind array buffer for vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[mesh->meshID]);
    //copy data from vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh->vertices.size(),
                 &(mesh->vertices)[0], GL_STATIC_DRAW);
    //bind buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[mesh->meshID]);
    //copy data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->indices.size(),
                 &(mesh->indices)[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void initPrimitive(Primitives primitive) {
    switch (primitive) {
        case Triangles:
            glBindVertexArray(VAOs[TriangleID]);
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[TriangleID]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(vPosition);
            break;
    }
}

void displayMesh(Mesh *mesh) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindVertexArray(VAOs[mesh->meshID]);
    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

void displayPrimitive(Primitives primitive) {
    switch (primitive) {
        case Triangles:
            glBindVertexArray(VAOs[TriangleID]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            break;
    }
}






































