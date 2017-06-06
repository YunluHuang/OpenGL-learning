//
//  PointLight.hpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef PointLight_h
#define PointLight_h

#include "Light.hpp"

struct PointLight : public Light {
    
    // Point Light Projection Matrix
    const static mat4 projection;
    
    // Position of the point light
    vec3 pos;
    
    // Constructor
    PointLight(vec3 pos, vec3 color);
    
    // Helper methods
    virtual vec4 getLightPosition();
    virtual mat4 getLightSpace();
};


#endif /* PointLight_h */
