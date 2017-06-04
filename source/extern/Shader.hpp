//
//  Shader.hpp
//  CSE163-final
//
//  Created by Liby Lee on 6/4/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

#include <OpenGL/gl3.h>
#include <vector>

#include "math.hpp"

struct Shader {
    
    // Constructor
    Shader(const char * vertShader, const char * fragShader);
    
    // Program Id
    unsigned int id;
    
    // Switch to this shader
    void use();
    GLuint getUniId(const char * name);
    
    // Set shader variable
    void set(const char * name, const GLint i);
    void set(const char * name, const GLuint i);
    void set(const char * name, const GLfloat f);
    
    void set(const char * name, const mat2 m);
    void set(const char * name, const mat3 m);
    void set(const char * name, const mat4 m);
    
    void set(const char * name, const vec2 v);
    void set(const char * name, const vec3 v);
    void set(const char * name, const vec4 v);
    
    void set(const char * name, const std::vector<vec2> & vs);
    void set(const char * name, const std::vector<vec3> & vs);
    void set(const char * name, const std::vector<vec4> & vs);
};



#endif /* Shader_h */
