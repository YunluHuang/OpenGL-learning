//
//  DirectLight.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "DirectLight.hpp"

const mat4 DirectLight::projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 10.0f);

DirectLight::DirectLight(vec3 dir, vec3 color) : Light(color) {
    this->dir = dir;
}

vec4 DirectLight::getLightPosition() {
    return vec4(dir, 0.0f);
}

mat4 DirectLight::getLightSpace() {
    vec3 lightPos = dir;
    vec3 upTest = cross(lightPos, vec3(0, 1, 0));
    vec3 lightUp = upTest == vec3(0, 0, 0) ? vec3(1, 0, 0) : vec3(0, 1, 0);
    mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), lightUp);
    return projection * lightView;
}
