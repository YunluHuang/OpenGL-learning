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
    Object(Mesh * mesh, mat4 transf);
    Mesh * mesh;
    mat4 transf;
};

#endif /* Object_h */
