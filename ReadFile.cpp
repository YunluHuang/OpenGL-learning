//
//  ReadFile.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "ReadFile.hpp"

using namespace std;

int numTypes = 0;
vector<GLuint> VAOs;
vector<GLuint> VBOs;
vector<GLuint> NBOs;
vector<GLuint> EBOs;

std::vector<Mesh *> Meshes;

unsigned int TriangleID = 0;

void add() {
    numTypes++;
    VAOs.push_back(0);
    VBOs.push_back(0);
    EBOs.push_back(0);
}

void readfile() {
    Mesh *mesh = new Mesh{
        static_cast<unsigned int>(numTypes),
        "teapot.obj",
        vector<vec3>(),
        vector<vec3>(),
        vector<GLuint>()
    };
    Meshes.push_back(mesh);
    add();
    TriangleID = numTypes;
    add();
}
