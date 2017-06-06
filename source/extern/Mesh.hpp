//
//  Mesh.hpp
//  CSE163-final
//
//  Created by Liby Lee on 6/2/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <OpenGL/gl3.h>

struct Mesh {
    
    // Mesh id
    int id;
    
    Mesh(const char * filename);
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> indices;
};

#endif /* Mesh_hpp */
