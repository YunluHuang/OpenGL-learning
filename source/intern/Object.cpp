//
//  Object.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "Object.hpp"

Object::Object(Mesh * mesh, mat4 transf) {
    this->mesh = mesh;
    this->transf = mat4(transf);
}
