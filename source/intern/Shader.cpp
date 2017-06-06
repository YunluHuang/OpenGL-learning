//
//  Shader.cpp
//  CSE163-final
//
//  Created by Liby Lee on 6/4/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.hpp"

using namespace std;

void checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- "
                      << std::endl;
            exit(1);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- "
                      << std::endl;
            exit(1);
        }
    }
}

Shader::Shader(const char * vertShaderPath, const char * fragShaderPath) {
    
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertShaderPath);
        fShaderFile.open(fragShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    
    // 2. compile shaders
    unsigned int vertex, fragment;
    
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // shader Program
    this->id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");
    
    // delete the shaders as they're linked into our program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const char * vertShaderPath, const char * fragShaderPath, const char * geoShaderPath) {
    
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        // open files
        vShaderFile.open(vertShaderPath);
        fShaderFile.open(fragShaderPath);
        gShaderFile.open(geoShaderPath);
        std::stringstream vShaderStream, fShaderStream, gShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        gShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        geometryCode = gShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    const char * gShaderCode = geometryCode.c_str();
    
    // 2. compile shaders
    unsigned int vertex, fragment, geometry;
    
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // geometry shader
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    checkCompileErrors(geometry, "GEOMETRY");
    
    // shader Program
    this->id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glAttachShader(id, geometry);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");
    
    // delete the shaders as they're linked into our program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);
}

void Shader::use() {
    glUseProgram(this->id);
}

GLuint Shader::getUniId(const char * name) {
    return glGetUniformLocation(this->id, name);
}

void Shader::set(const char * name, const GLint i) {
    glUniform1i(getUniId(name), i);
}

void Shader::set(const char * name, const GLuint i) {
    glUniform1ui(getUniId(name), i);
}

void Shader::set(const char * name, const GLfloat f) {
    glUniform1f(getUniId(name), f);
}

void Shader::set(const char * name, const vec2 v) {
    glUniform2fv(getUniId(name), 1, &v[0]);
}

void Shader::set(const char * name, const vec3 v) {
    glUniform3fv(getUniId(name), 1, &v[0]);
}

void Shader::set(const char * name, const vec4 v) {
    glUniform4fv(getUniId(name), 1, &v[0]);
}

void Shader::set(const char * name, const mat2 m) {
    glUniformMatrix2fv(getUniId(name), 1, GL_FALSE, &m[0][0]);
}

void Shader::set(const char * name, const mat3 m) {
    glUniformMatrix3fv(getUniId(name), 1, GL_FALSE, &m[0][0]);
}

void Shader::set(const char * name, const mat4 m) {
    glUniformMatrix4fv(getUniId(name), 1, GL_FALSE, &m[0][0]);
}

void Shader::set(const char * name, const std::vector<vec2> & vs) {
    vector<GLfloat> v;
    for (int i = 0; i < vs.size(); i++) {
        v.push_back(vs[i][0]);
        v.push_back(vs[i][1]);
    }
    glUniform2fv(getUniId(name), (int) vs.size(), &v[0]);
}

void Shader::set(const char * name, const std::vector<vec3> & vs) {
    vector<GLfloat> v;
    for (int i = 0; i < vs.size(); i++) {
        v.push_back(vs[i][0]);
        v.push_back(vs[i][1]);
        v.push_back(vs[i][2]);
    }
    glUniform3fv(getUniId(name), (int) vs.size(), &v[0]);
}

void Shader::set(const char * name, const std::vector<vec4> & vs) {
    vector<GLfloat> v;
    for (int i = 0; i < vs.size(); i++) {
        v.push_back(vs[i][0]);
        v.push_back(vs[i][1]);
        v.push_back(vs[i][2]);
        v.push_back(vs[i][3]);
    }
    glUniform4fv(getUniId(name), (int) vs.size(), &v[0]);
}

void Shader::set(const char * name, const std::vector<mat2> & ms) {
    vector<GLfloat> v;
    for (int i = 0; i < ms.size(); i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                v.push_back(ms[i][j][k]);
            }
        }
    }
    glUniformMatrix2fv(getUniId(name), (int) ms.size(), GL_FALSE, &v[0]);
}

void Shader::set(const char * name, const std::vector<mat3> & ms) {
    vector<GLfloat> v;
    for (int i = 0; i < ms.size(); i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                v.push_back(ms[i][j][k]);
            }
        }
    }
    glUniformMatrix2fv(getUniId(name), (int) ms.size(), GL_FALSE, &v[0]);
}

void Shader::set(const char * name, const std::vector<mat4> & ms) {
    vector<GLfloat> v;
    for (int i = 0; i < ms.size(); i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                v.push_back(ms[i][j][k]);
            }
        }
    }
    glUniformMatrix2fv(getUniId(name), (int) ms.size(), GL_FALSE, &v[0]);
}
