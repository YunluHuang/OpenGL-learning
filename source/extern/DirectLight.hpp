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
    DirectLight(vec3 dir, vec3 color);
    vec3 dir;
};


#endif /* DirectLight_h */
