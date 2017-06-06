//
//  DirectLight.hpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef DirectLight_h
#define DirectLight_h

#include "Light.hpp"

struct DirectLight : public Light {
    
    // Direct Light Projection Matrix
    static const mat4 projection;
    
    // Direct Light Parameters
    vec3 dir;
    
    // Constructor
    DirectLight(vec3 dir, vec3 color);
    
    // Helper functions
    virtual vec4 getLightPosition();
    virtual mat4 getLightSpace();
};


#endif /* DirectLight_h */
