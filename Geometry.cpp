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
vector<vec3> normals;
vector<GLuint> indices;

void initGeometry(char *fileName) {
    parseFile(fileName);
    loadObj();
    bindBuffer();
}

void parseFile(char *fileName) {
    FILE* fp;
    float x, y, z;
    int fx, fy, fz, ignore;
    int c1, c2;
    float minY = INFINITY, minZ = INFINITY;
    float maxY = -INFINITY, maxZ = -INFINITY;
    
    fp = fopen(fileName, "rb");
    
    if (fp == NULL) {
        cerr << "Error loading file: " << fileName << endl;
        exit(-1);
    }
    
    while (!feof(fp)) {
        c1 = fgetc(fp);
        while (!(c1 == 'v' || c1 == 'f')) {
            c1 = fgetc(fp);
            if (feof(fp))
                break;
        }
        c2 = fgetc(fp);
        
        if ((c1 == 'v') && (c2 == ' ')) {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            vertices.push_back(vec3(x, y, z));
            if (y < minY) minY = y;
            if (z < minZ) minZ = z;
            if (y > maxY) maxY = y;
            if (z > maxZ) maxZ = z;
        }
        else if ((c1 == 'v') && (c2 == 'n')) {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            // Ignore the normals in mytest2, as we use a solid color for the teapot.
            normals.push_back(glm::normalize(vec3(x, y, z)));
        }
        else if (c1 == 'f')
        {
            fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
            indices.push_back(fx - 1);
            indices.push_back(fy - 1);
            indices.push_back(fz - 1);
        }
    }
    
    fclose(fp);

}

/**
    set all data here
 */
void loadObj() {
    
//    //initialize the triagnle's vertices
//    vertices.push_back(vec3(-0.9f, -0.9f, 0.0f));
//    vertices.push_back(vec3(0.85f, -0.9f, 0.0f));
//    vertices.push_back(vec3(-0.9f, 0.85f, 0.0f));
//    vertices.push_back(vec3(0.9f, -0.85f, 0.0f));
//    vertices.push_back(vec3(0.9f, 0.9f, 0.0f));
//    vertices.push_back(vec3(-0.85f, 0.9f, 0.0f));
//    
//    //initialize the faces
//    for(int i = 0; i < vertices.size(); i++) {
//        indices.push_back(i);
//    }
    
    //compute normal
    
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






































