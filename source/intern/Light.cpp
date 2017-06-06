//
//  Light.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "Light.hpp"

Light::Light(vec3 color) {
    this->color = color;
}

vec4 Light::getLightPosition() {
    return vec4();
}

mat4 Light::getLightSpace() {
    return mat4();
}
