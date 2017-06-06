//
//  PointLight.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "PointLight.hpp"

const mat4 PointLight::projection = glm::perspective(PI, 1.0f, 0.1f, 10.0f);

PointLight::PointLight(vec3 pos, vec3 color) : Light(color) {
    
    this->pos = pos;
    
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 256, 256, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
