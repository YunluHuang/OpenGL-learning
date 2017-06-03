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

void addObject(const char * filename) {
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
    
    FILE * fp;
    string line;
    size_t length = 0;
    char * buf;
    
    fp = fopen(filename, "rb");
    
    if (fp == NULL) {
        cerr << "Error loading scene file: " << filename << endl;
        exit(-1);
    }
    
    while (!feof(fp)) {
        
        // Get Line String
        buf = fgetln(fp, &length);
        if (!buf) {
            continue;
        }
        
        // Get command (the first
        string command = strtok(buf, " \n");
        if (command == "object") {
            char * objname = strtok(nullptr, " \n");
            addObject(objname);
        }
        else {
            cerr << "Unknown Command in Scene File: " << command << endl;
            exit(-1);
        }
    }
}
