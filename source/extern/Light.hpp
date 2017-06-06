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
#include <vector>

#include "math.hpp"



struct Light {
    
    static int count;
    static float borderColor[4];
    
    //
    int id;
    GLuint depthMap;
    GLuint depthMapFBO;
    vec3 color;
    
    // Constructor
    Light(vec3 color);
    
    // Virtual helper methods
    virtual vec4 getLightPosition();
    virtual mat4 getLightSpace();
};

#endif /* Light_hpp */
