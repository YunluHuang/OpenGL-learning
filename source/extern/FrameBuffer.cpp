//
//  FrameBuffer.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "FrameBuffer.hpp"

using namespace std;

vector<GLuint> FBOs;

void genFBOs(unsigned int w, unsigned int h) {
    
    glGenFramebuffers(1, &FBOs[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);
    
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
    glDrawBuffer(GL_NONE);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cerr << "Frame buffer is not complete!" << endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}
