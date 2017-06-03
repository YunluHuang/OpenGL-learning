//
//  Object.hpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef Object_h
#define Object_h

#include "math.hpp"
#include "Mesh.hpp"

struct Object {
    
    // Constructor
    Object(Mesh * mesh, mat4 transf, vec3 ambient, vec3 diffuse, vec3 specular, float shininess);
    
    // Mesh ref
    Mesh * mesh;
    
    // Transformation Matrix
    mat4 transf;
    
    // Color variables;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

#endif /* Object_h */
