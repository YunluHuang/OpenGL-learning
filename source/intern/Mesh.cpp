//
//  Mesh.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/2/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "math.hpp"
#include "Mesh.hpp"

using namespace std;

#include <fstream>

Mesh::Mesh(const char * filename) {
    
    vector<vec3> verts;
    vector<vec3> norms;
    
    string line;
    size_t bufSize = 128;
    char buf[bufSize];
    
    std::ifstream ifs (filename, std::ifstream::in | std::ifstream::binary);
    
    if (!ifs.is_open()) {
        cerr << "Error loading object file: " << filename << endl;
        exit(-1);
    }
    
    while (!ifs.eof()) {
        
        // Get Line String
        ifs.getline(buf, bufSize);
        line = buf;
        if (line == "") {
            continue;
        }
        
        // Get command (the first
        string command = strtok(buf, " \n");
        if (command == "v") {
            float x = atof(strtok(nullptr, " \n"));
            float y = atof(strtok(nullptr, " \n"));
            float z = atof(strtok(nullptr, " \n"));
            verts.push_back(vec3(x, y, z));
        }
        else if (command == "vn") {
            float x = atof(strtok(nullptr, " \n"));
            float y = atof(strtok(nullptr, " \n"));
            float z = atof(strtok(nullptr, " \n"));
            norms.push_back(normalize(vec3(x, y, z)));
        }
        else if (command == "f") {
            
            int fx = atoi(strtok(buf + 2, " /\n"));
            int nx = atoi(strtok(nullptr, " /\n"));
            int fy = atoi(strtok(nullptr, " /\n"));
            int ny = atoi(strtok(nullptr, " /\n"));
            int fz = atoi(strtok(nullptr, " /\n"));
            int nz = atoi(strtok(nullptr, " /\n"));
            
            vertices.push_back(verts[fx - 1]);
            vertices.push_back(verts[fy - 1]);
            vertices.push_back(verts[fz - 1]);
            
            normals.push_back(norms[nx - 1]);
            normals.push_back(norms[ny - 1]);
            normals.push_back(norms[nz - 1]);
            
            GLuint offset = (unsigned int) indices.size();
            indices.push_back(offset);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);
        }
    }
}
