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
GLuint depthMap;
GLuint FBO;

unsigned int shadowWidth = 1024;
unsigned int shadowHeight = 1024;

void printMat4(mat4 & m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << m[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

void renderShadow() {
    
    //generate FBO (TODO: only 1 for depth map)
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    
    //generate depthTexture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    
    //set color buffer to none
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cerr << "Frame buffer is not complete!" << endl;
    }
    
//    genFBOs(width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
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
    float nearPlane = -10.0f;
    float farPlane = 20.0f;
    mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
    
    cout << "lightProjection: " << endl;
    printMat4(lightProjection);

    mat4 lightView = glm::lookAt(-light->dir, center, vec3(0.0f, 1.0f, 0.0f));
    cout << "lightView: " << endl;
    printMat4(lightView);
    
    mat4 lightProjectionView = lightProjection * lightView;
    return lightProjectionView;
}

void renderObjShadow(Object *obj, Light *light) {
    
    mat4 lightMVP = computeLightSpace(light) * obj->transf;
    
    cout << "lightMVP: " << endl;
    printMat4(lightMVP);
    
    GLuint lightMVPPos = glGetUniformLocation(shadowProgram, "lightMVP");
    glUniformMatrix4fv(lightMVPPos, 1, GL_FALSE, &lightMVP[0][0]);
    
    glViewport(0, 0, width, height);
    
    displayObject(obj);
    
}
