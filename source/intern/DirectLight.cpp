//
//  DirectLight.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "DirectLight.hpp"

const mat4 DirectLight::projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 10.0f);

DirectLight::DirectLight(vec3 dir, vec3 color) : Light(color) {
    
    this->dir = dir;
    
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

vec4 DirectLight::getLightPosition() {
    return vec4(dir, 0.0f);
}

mat4 DirectLight::getLightSpace() {
    vec3 lightPos = dir;
    vec3 upTest = cross(lightPos, vec3(0, 1, 0));
    vec3 lightUp = upTest == vec3(0, 0, 0) ? vec3(1, 0, 0) : vec3(0, 1, 0);
    mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), lightUp);
    return projection * lightView;
}
