//
//  ReadFile.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "ReadFile.hpp"
#include "Mesh.hpp"

using namespace std;

int numTypes = 0;
vector<GLuint> VAOs;
vector<GLuint> VBOs;
vector<GLuint> NBOs;
vector<GLuint> EBOs;

std::vector<Mesh *> objects;
std::map<string, Mesh *> loadedMeshes;

void addMesh(const char * filename) {
    string file = filename;
    auto it = loadedMeshes.find(file);
    if (it == loadedMeshes.end()) {
        Mesh * mesh = new Mesh(filename);
        mesh->id = loadedMeshes.size();
        loadedMeshes[file] = mesh;
        objects.push_back(mesh);
        VAOs.push_back(0);
        VBOs.push_back(0);
        EBOs.push_back(0);
    }
    else {
        objects.push_back(it->second);
    }
}

void readfile(const char * filename) {
    addMesh("teapot.obj");
}
