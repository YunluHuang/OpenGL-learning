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
    glGenVertexArrays(numObjs, &VAOs[0]);
    glGenBuffers(numObjs, &VBOs[0]);
    glGenBuffers(numObjs, &EBOs[0]);
}

void initMesh(Mesh *mesh) {
    loadMesh(mesh);
    bindBuffers(mesh);
}

//void parseFile(Mesh *mesh) {
//    FILE* fp;
//    float x, y, z;
//    int fx, fy, fz, ignore;
//    int c1, c2;
//    float minY = INFINITY, minZ = INFINITY;
//    float maxY = -INFINITY, maxZ = -INFINITY;
//    
//    fp = fopen(mesh->fileName, "rb");
//    
//    if (fp == NULL) {
//        cerr << "Error loading file: " << mesh->fileName << endl;
//        exit(-1);
//    }
//    
//    while (!feof(fp)) {
//        c1 = fgetc(fp);
//        while (!(c1 == 'v' || c1 == 'f')) {
//            c1 = fgetc(fp);
//            if (feof(fp))
//                break;
//        }
//        c2 = fgetc(fp);
//        
//        if ((c1 == 'v') && (c2 == ' ')) {
//            fscanf(fp, "%f %f %f", &x, &y, &z);
//            mesh->vertices.push_back(vec3(x, y, z));
//            if (y < minY) minY = y;
//            if (z < minZ) minZ = z;
//            if (y > maxY) maxY = y;
//            if (z > maxZ) maxZ = z;
//        }
//        else if ((c1 == 'v') && (c2 == 'n')) {
//            fscanf(fp, "%f %f %f", &x, &y, &z);
//            // Ignore the normals in mytest2, as we use a solid color for the teapot.
//            mesh->normals.push_back(glm::normalize(vec3(x, y, z)));
//        }
//        else if (c1 == 'f')
//        {
//            fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
//            mesh->indices.push_back(fx - 1);
//            mesh->indices.push_back(fy - 1);
//            mesh->indices.push_back(fz - 1);
//        }
//    }
//    
//    fclose(fp);
//
//}
//
///**
//    set all data here
// */
//void loadMesh(Mesh *mesh) {
//    
////    //initialize the triagnle's vertices
////    vertices.push_back(vec3(-0.9f, -0.9f, 0.0f));
////    vertices.push_back(vec3(0.85f, -0.9f, 0.0f));
////    vertices.push_back(vec3(-0.9f, 0.85f, 0.0f));
////    vertices.push_back(vec3(0.9f, -0.85f, 0.0f));
////    vertices.push_back(vec3(0.9f, 0.9f, 0.0f));
////    vertices.push_back(vec3(-0.85f, 0.9f, 0.0f));
////    
////    //initialize the faces
////    for(int i = 0; i < vertices.size(); i++) {
////        indices.push_back(i);
////    }
//    
//    //bind VAOs and buffers
//    glBindVertexArray(VAOs[mesh->meshID]);
//    //bind array buffer for vertices
//    glBindBuffer(GL_ARRAY_BUFFER, VBOs[mesh->meshID]);
//    //copy data from vertices to buffer
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh->vertices.size(),
//                 &(mesh->vertices)[0], GL_STATIC_DRAW);
//    //bind buffer
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[mesh->meshID]);
//    //copy data
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->indices.size(),
//                 &(mesh->indices)[0], GL_STATIC_DRAW);
//    
//    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(vPosition);
//}

/**
 set all data here
 */
void bindBuffers(Mesh *mesh) {
    
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
}

void displayMesh(Mesh *mesh) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindVertexArray(VAOs[mesh->meshID]);
    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}






































