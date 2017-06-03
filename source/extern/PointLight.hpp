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
    PointLight(vec3 pos, vec3 color);
    vec3 pos;
};


#endif /* PointLight_h */
