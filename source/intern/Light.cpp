//
//  Light.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "Light.hpp"

int Light::count = 0;
float Light::borderColor[4] = { 1.0, 1.0, 1.0, 1.0 };

Light::Light(vec3 color) {
    this->color = color;
    this->id = count;
    count++;
}

vec4 Light::getLightPosition() {
    return vec4();
}

mat4 Light::getLightSpace() {
    return mat4();
}
