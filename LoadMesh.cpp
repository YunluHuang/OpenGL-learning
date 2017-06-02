//
//  LoadMesh.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/2/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "LoadMesh.hpp"

using namespace std;

void loadMesh(Mesh *mesh) {
    FILE* fp;
    float x, y, z;
    int fx, fy, fz, ignore;
    int c1, c2;
    float minY = INFINITY, minZ = INFINITY;
    float maxY = -INFINITY, maxZ = -INFINITY;
    
    fp = fopen(mesh->fileName, "rb");
    
    if (fp == NULL) {
        cerr << "Error loading file: " << mesh->fileName << endl;
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
            mesh->vertices.push_back(vec3(x, y, z));
            if (y < minY) minY = y;
            if (z < minZ) minZ = z;
            if (y > maxY) maxY = y;
            if (z > maxZ) maxZ = z;
        }
        else if ((c1 == 'v') && (c2 == 'n')) {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            // Ignore the normals in mytest2, as we use a solid color for the teapot.
            mesh->normals.push_back(glm::normalize(vec3(x, y, z)));
        }
        else if (c1 == 'f')
        {
            fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
            mesh->indices.push_back(fx - 1);
            mesh->indices.push_back(fy - 1);
            mesh->indices.push_back(fz - 1);
        }
    }
    
    fclose(fp);
}
