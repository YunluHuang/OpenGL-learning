//
//  LoadShader.hpp
//  openGL_tutorial_1
//
//  Created by Ah Huang on 5/31/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef LoadShader_hpp
#define LoadShader_hpp

#include <stdio.h>
#include <OpenGL/gl3.h>

typedef struct {
    GLenum type;
    const char *filename;
    GLuint shader;
} ShaderInfo;

GLuint LoadShaders( ShaderInfo *);

#endif /* LoadShader_hpp */
