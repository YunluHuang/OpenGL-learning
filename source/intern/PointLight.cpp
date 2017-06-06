//
//  PointLight.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "PointLight.hpp"

const mat4 PointLight::projection = glm::perspective(2 * PI, 1.0f, 0.1f, 10.0f);

PointLight::PointLight(vec3 pos, vec3 color) : Light(color) {
    this->pos = pos;
}

vec4 PointLight::getLightPosition() {
    return vec4(pos, 1.0f);
}

mat4 PointLight::getLightSpace() {
    vec3 lightPos = pos;
    vec3 upTest = cross(lightPos, vec3(0, 1, 0));
    vec3 lightUp = upTest == vec3(0, 0, 0) ? vec3(1, 0, 0) : vec3(0, 1, 0);
    mat4 lightView = glm::lookAt(lightPos, lightPos + vec3(1, 0, 0), lightUp);
    return projection * lightView;
}
