//
//  FrameBuffer.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/3/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "FrameBuffer.hpp"

using namespace std;

//vector<GLuint> FBOs;

void genFBOs(unsigned int w, unsigned int h) {
//    FBOs.push_back(0);
//    
//    //generate FBO (TODO: only 1 for depth map)
//    glGenFramebuffers(1, &FBOs[0]);
//    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);
//    
//    //generate depthTexture
//    glGenTextures(1, &depthMap);
//    glBindTexture(GL_TEXTURE_2D, depthMap);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    
//    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
//    
//    //set color buffer to none
//    glDrawBuffer(GL_NONE);
//    glReadBuffer(GL_NONE);
//    
//    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//        cerr << "Frame buffer is not complete!" << endl;
//    }
    
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}
