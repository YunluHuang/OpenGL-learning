//
//  Light.hpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include <stdio.h>

#include "math.hpp"

struct Light {
    Light(vec3 color);
    virtual vec4 getLightPosition();
    virtual mat4 getLightSpace();
    vec3 color;
};

#endif /* Light_hpp */
