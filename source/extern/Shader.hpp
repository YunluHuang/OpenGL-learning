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
#include <string>

#include "math.hpp"

struct Shader {
    
    // Constructor
    Shader(const char * vertShader, const char * fragShader);
    Shader(const char * vertShader, const char * fragShader, const char * geoShaderPath);
    
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
    
    void set(const char * name, const std::vector<mat2> & ms);
    void set(const char * name, const std::vector<mat3> & ms);
    void set(const char * name, const std::vector<mat4> & ms);
    
    // Set array variable element with array index
    void set(const char * name, int index, const GLint i);
    void set(const char * name, int index, const GLuint i);
    void set(const char * name, int index, const GLfloat i);
    
    void set(const char * name, int index, const mat2 i);
    void set(const char * name, int index, const mat3 i);
    void set(const char * name, int index, const mat4 i);
    
    void set(const char * name, int index, const vec2 i);
    void set(const char * name, int index, const vec3 i);
    void set(const char * name, int index, const vec4 i);
};



#endif /* Shader_h */
