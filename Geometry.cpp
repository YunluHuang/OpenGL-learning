//
//  Geometry.cpp
//  openGL_tutorial_1
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "Geometry.hpp"

using namespace std;

GLuint VAOs[NumObjs];
GLuint VBOs[NumObjs];
GLuint EBOs[NumObjs];

vector<vec3> vertices;
vector<GLuint> indices;

void initGeometry() {
    loadObj();
    bindBuffer();
}

/**
    set all data here
 */
void loadObj() {
    
    //initialize the triagnle's vertices
    vertices.push_back(vec3(-0.9f, -0.9f, 0.0f));
    vertices.push_back(vec3(0.85f, -0.9f, 0.0f));
    vertices.push_back(vec3(-0.9f, 0.85f, 0.0f));
    vertices.push_back(vec3(0.9f, -0.85f, 0.0f));
    vertices.push_back(vec3(0.9f, 0.9f, 0.0f));
    vertices.push_back(vec3(-0.85f, 0.9f, 0.0f));
    
    //initialize the faces
    for(int i = 0; i < vertices.size(); i++) {
        indices.push_back(i);
    }
    
}

/**
    send the data to OpenGL
 */
void bindBuffer() {
    //create VAO for our triangles (one obj)
    glGenVertexArrays(NumObjs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    
    //generate 1 VBO
    glGenBuffers(NumObjs, VBOs);
    //bind array buffer for vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[Triangles]);
    //copy data from vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(),
                 &vertices[0], GL_STATIC_DRAW);
    
    //bind vertex position to vertex attribute
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);
    
    //generate 1 EBO
    glGenBuffers(NumObjs, EBOs);
    //bind buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[Triangles]);
    //copy data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void displayGeometry() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindVertexArray(VAOs[Triangles]);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}






































