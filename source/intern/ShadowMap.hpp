//
//  ShadowMap.hpp
//  CSE163-final
//
//  Created by Ah Huang on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef ShadowMap_hpp
#define ShadowMap_hpp

#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <iostream>
#include "DirectLight.hpp"
#include "variables.h"
//#include "FrameBuffer.hpp"
#include "LoadShader.hpp"
#include "Geometry.hpp"

void renderShadow();
mat4 computeLightSpace(DirectLight *generalLight);
void renderObjShadow(Object *obj, Light *light);

#endif /* ShadowMap_hpp */
