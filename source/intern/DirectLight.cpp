//
//  DirectLight.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "DirectLight.hpp"

DirectLight::DirectLight(vec3 dir, vec3 color) : Light(color) {
    this->dir = dir;
}
