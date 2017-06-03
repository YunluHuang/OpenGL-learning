//
//  ReadFile.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/1/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "math.hpp"
#include "Light.hpp"
#include "PointLight.hpp"
#include "DirectLight.hpp"
#include "ReadFile.hpp"
#include "Mesh.hpp"

using namespace std;

int numTypes = 0;
vector<GLuint> VAOs;
vector<GLuint> VBOs;
vector<GLuint> NBOs;
vector<GLuint> EBOs;

std::vector<Object *> objects;
std::map<string, Mesh *> loadedMeshes;
std::vector<Light *> lights;

std::stack<mat4> transfStack;

void addObject(const char * filename) {
    string file = filename;
    Mesh * mesh;
    auto it = loadedMeshes.find(file);
    if (it == loadedMeshes.end()) {
        mesh = new Mesh(filename);
        mesh->id = loadedMeshes.size();
        loadedMeshes[file] = mesh;
        VAOs.push_back(0);
        VBOs.push_back(0);
        NBOs.push_back(0);
        EBOs.push_back(0);
    }
    else {
        mesh = it->second;
    }
    Object * obj = new Object(mesh, transfStack.top());
    objects.push_back(obj);
}

void addPointLight(vec3 pos, vec3 color) {
    lights.push_back(new PointLight(pos, color));
}

void addDirectLight(vec3 dir, vec3 color) {
    lights.push_back(new DirectLight(dir, color));
}

void readfile(const char * filename) {
    
    transfStack.push(mat4(1.0));
    
    string line;
    size_t bufSize = 128;
    char buf[bufSize];
    
    std::ifstream ifs (filename, std::ifstream::in | std::ifstream::binary);
    
    if (!ifs.is_open()) {
        cerr << "Error loading scene file: " << filename << endl;
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
        if (command == "#") {
            continue;
        }
        else if (command == "object") {
            char * objname = strtok(nullptr, " \n");
            addObject(objname);
        }
        else if (command == "pointlight") {
            float x = atof(strtok(nullptr, " \n"));
            float y = atof(strtok(nullptr, " \n"));
            float z = atof(strtok(nullptr, " \n"));
            float r = atof(strtok(nullptr, " \n"));
            float g = atof(strtok(nullptr, " \n"));
            float b = atof(strtok(nullptr, " \n"));
            addPointLight(vec3(x, y, z), vec3(r, g, b));
        }
        else if (command == "directlight") {
            float x = atof(strtok(nullptr, " \n"));
            float y = atof(strtok(nullptr, " \n"));
            float z = atof(strtok(nullptr, " \n"));
            float r = atof(strtok(nullptr, " \n"));
            float g = atof(strtok(nullptr, " \n"));
            float b = atof(strtok(nullptr, " \n"));
            addDirectLight(vec3(x, y, z), vec3(r, g, b));
        }
        else if (command == "translate") {
            float x = atof(strtok(nullptr, " \n"));
            float y = atof(strtok(nullptr, " \n"));
            float z = atof(strtok(nullptr, " \n"));
            transfStack.top() *= glm::translate(mat4(1.0f), vec3(x, y, z));
        }
        else if (command == "scale") {
            float x = atof(strtok(nullptr, " \n"));
            float y = atof(strtok(nullptr, " \n"));
            float z = atof(strtok(nullptr, " \n"));
            transfStack.top() *= glm::scale(mat4(1.0f), vec3(x, y, z));
        }
        else if (command == "rotateX") {
            float x = atof(strtok(nullptr, " \n"));
            transfStack.top() *= glm::rotate(mat4(1.0f), x, vec3(1, 0, 0));
        }
        else if (command == "rotateY") {
            float y = atof(strtok(nullptr, " \n"));
            transfStack.top() *= glm::rotate(mat4(1.0f), y, vec3(0, 1, 0));
        }
        else if (command == "rotateZ") {
            float z = atof(strtok(nullptr, " \n"));
            transfStack.top() *= glm::rotate(mat4(1.0f), z, vec3(0, 0, 1));
        }
        else if (command == "pushtransf") {
            transfStack.push(transfStack.top());
            cout << "pushing, size " << transfStack.size() << endl;
        }
        else if (command == "poptransf") {
            if (transfStack.empty()) {
                cout << "cannot pop from stack" << endl;
                continue;
            }
            transfStack.pop();
            cout << "popping, size " << transfStack.size() << endl;
        }
        else {
            cerr << "Unknown Command in Scene File: " << command << endl;
        }
    }
}
