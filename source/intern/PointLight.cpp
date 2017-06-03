//
//  PointLight.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "PointLight.hpp"

PointLight::PointLight(vec3 pos, vec3 color) : Light(color) {
    this->pos = pos;
}
