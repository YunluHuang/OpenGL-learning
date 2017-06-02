//
//  ReadFile.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "ReadFile.hpp"

using namespace std;

int numObjs = 1;
vector<GLuint> VAOs;
vector<GLuint> VBOs;
vector<GLuint> NBOs;
vector<GLuint> EBOs;

//vector<vector<vec3>> objVertices;
//vector<vector<vec3>> objNormals;
//vector<vector<GLuint>> objIndices;
std::vector<Mesh *> Meshes;

void addMesh(unsigned int meshID) {
    VAOs.push_back(meshID);
    VBOs.push_back(meshID);
    EBOs.push_back(meshID);
}

void readfile() {
    Mesh *mesh = new Mesh{
        0,
        "teapot.obj",
        vector<vec3>(),
        vector<vec3>(),
        vector<GLuint>()
    };
    Meshes.push_back(mesh);
    addMesh(mesh->meshID);
}
