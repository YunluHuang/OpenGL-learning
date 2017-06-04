//
//  ShadowMap.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "ShadowMap.hpp"

using namespace std;

GLuint shadowProgram;

unsigned int shadowWidth = 1024;
unsigned int shadowHeight = 1024;

void renderShadow() {
    
    genFBOs(shadowWidth, shadowHeight);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
    ShaderInfo depthShaders[] = {
        {GL_VERTEX_SHADER, "depth.vert.glsl"},
        {GL_FRAGMENT_SHADER, "depth.frag.glsl"},
        {GL_NONE, NULL}
    };
    
    shadowProgram = loadShaders(depthShaders);
    glUseProgram(shadowProgram);
    
    for (int i = 0; i < objects.size(); i++) {
        renderObjShadow(objects[i], lights[0]);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

mat4 computeLightSpace(Light *generalLight) {
    DirectLight *light = static_cast<DirectLight *>(generalLight);
    float nearPlane = 1.0f;
    float farPlane = 7.5f;
    mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
    cout << "lightDir = (" << light->dir.x << ", " << light->dir.y << ", "
    << light->dir.z << ")" << endl;
    mat4 lightView = glm::lookAt(-light->dir, center, vec3(0.0f, 1.0f, 0.0f));
    mat4 lightProjectionView = lightProjection * lightView;
    return lightProjectionView;
}

void renderObjShadow(Object *obj, Light *light) {
    
    mat4 lightProjectionView = computeLightSpace(light);
    
    
    GLuint lightPVPos = glGetUniformLocation(shadowProgram, "lightProjectionView");
    glUniformMatrix4fv(lightPVPos, 1, GL_FALSE, &lightProjectionView[0][0]);
    GLuint meshModelPos = glGetUniformLocation(shadowProgram, "meshModel");
    mat4 meshModel = obj->transf;
    glUniformMatrix4fv(meshModelPos, 1, GL_FALSE, &meshModel[0][0]);
    
    glViewport(0, 0, shadowWidth, shadowHeight);
    
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    displayObject(obj);
    
}
