//
//  Mesh.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/2/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "Mesh.hpp"

using namespace std;

Mesh::Mesh(const char * filename) {
    FILE* fp;
    float x, y, z;
    int fx, fy, fz, ignore;
    int c1, c2;
    float minY = INFINITY, minZ = INFINITY;
    float maxY = -INFINITY, maxZ = -INFINITY;
    
    fp = fopen(filename, "rb");
    
    if (fp == NULL) {
        cerr << "Error loading file: " << filename << endl;
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
            vertices.push_back(glm::vec3(x, y, z));
            if (y < minY) minY = y;
            if (z < minZ) minZ = z;
            if (y > maxY) maxY = y;
            if (z > maxZ) maxZ = z;
        }
        else if ((c1 == 'v') && (c2 == 'n')) {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            // Ignore the normals in mytest2, as we use a solid color for the teapot.
            normals.push_back(glm::normalize(glm::vec3(x, y, z)));
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
