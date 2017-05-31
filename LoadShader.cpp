//
//  LoadShader.cpp
//  openGL_tutorial_1
//
//  Created by Ah Huang on 5/31/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "LoadShader.hpp"
#include <cstdio>

static const GLchar * ReadShader(const char *filename) {
    FILE *infile = fopen(filename, "rb");
    
    if (!infile) {
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
        return NULL;
    }
    
    fseek(infile, 0, SEEK_END);
    long len = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    
    GLchar *source = new GLchar[len + 1];
    
    fread(source, 1, len, infile);
    fclose(infile);
    
    source[len] = 0;
    
    return const_cast<const GLchar *>(source);
}

GLuint LoadShaders(ShaderInfo *shaders) {
    if(shaders == NULL) return 0;
    
    GLuint program = glCreateProgram();
    
    ShaderInfo *entry = shaders;
    
    while(entry->type != GL_NONE) {
        GLuint shader = glCreateShader(entry->type);
        entry->shader = shader;
        
        const GLchar *source = ReadShader(entry->filename);
        if(source == NULL) {
            for(entry = shaders; entry->type != GL_NONE; ++entry) {
                glDeleteShader(entry->shader);
                entry->shader = 0;
            }
            std::cout << "no shader load" << std::endl;
            return 0;
        }
        
        glShaderSource(shader, 1, &source, NULL);
        delete[] source;
        
        glCompileShader(shader);
        
        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        
        if(!compiled) {
            return 0;
        }
        
        glAttachShader(program, shader);
        
        ++entry;
    }
    
    glLinkProgram(program);
    
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked) {
        for(entry = shaders; entry->type != GL_NONE; ++entry) {
            glDeleteShader(entry->shader);
            entry->shader = 0;
        }
        return 0;
    }
    
    return program;
}



////////////////////////////////////////////////////////////////////////////////
////
////  --- LoadShaders.cxx ---
////
////////////////////////////////////////////////////////////////////////////////
//
//#include <cstdlib>
//#include <iostream>
//
//#include "LoadShader.hpp"
//
//#ifdef __cplusplus
//extern "C" {
//#endif // __cplusplus
//    
//    //----------------------------------------------------------------------------
//    
//    static const GLchar*
//    ReadShader( const char* filename )
//    {
//        FILE* infile = fopen( filename, "rb" );
//        
//        if ( !infile ) {
//#ifdef DEBUG
//            std::cerr << "Unable to open file '" << filename << "'" << std::endl;
//#endif /* DEBUG */
//            return NULL;
//        }
//        
//        fseek( infile, 0, SEEK_END );
//        long len = ftell( infile );
//        fseek( infile, 0, SEEK_SET );
//        
//        GLchar* source = new GLchar[len+1];
//        
//        fread( source, 1, len, infile );
//        fclose( infile );
//        
//        source[len] = 0;
//        
//        return const_cast<const GLchar*>(source);
//    }
//    
//    //----------------------------------------------------------------------------
//    
//    GLuint
//    LoadShaders( ShaderInfo* shaders )
//    {
//        if ( shaders == NULL ) { return 0; }
//        
//        GLuint program = glCreateProgram();
//        
//        ShaderInfo* entry = shaders;
//        while ( entry->type != GL_NONE ) {
//            GLuint shader = glCreateShader( entry->type );
//            
//            entry->shader = shader;
//            
//            const GLchar* source = ReadShader( entry->filename );
//            if ( source == NULL ) {
//                for ( entry = shaders; entry->type != GL_NONE; ++entry ) {
//                    glDeleteShader( entry->shader );
//                    entry->shader = 0;
//                }
//                
//                return 0;
//            }
//            
//            glShaderSource( shader, 1, &source, NULL );
//            delete [] source;
//            
//            glCompileShader( shader );
//            
//            GLint compiled;
//            glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
//            if ( !compiled ) {
//#ifdef DEBUG
//                GLsizei len;
//                glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );
//                
//                GLchar* log = new GLchar[len+1];
//                glGetShaderInfoLog( shader, len, &len, log );
//                std::cerr << "Shader compilation failed: " << log << std::endl;
//                delete [] log;
//#endif /* DEBUG */
//                
//                return 0;
//            }
//            
//            glAttachShader( program, shader );
//            
//            ++entry;
//        }
//        
////#ifdef GL_VERSION_4_1
////        if ( GLEW_VERSION_4_1 ) {
////            // glProgramParameteri( program, GL_PROGRAM_SEPARABLE, GL_TRUE );
////        }
////#endif /* GL_VERSION_4_1 */
//        
//        glLinkProgram( program );
//        
//        GLint linked;
//        glGetProgramiv( program, GL_LINK_STATUS, &linked );
//        if ( !linked ) {
//#ifdef DEBUG
//            GLsizei len;
//            glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );
//            
//            GLchar* log = new GLchar[len+1];
//            glGetProgramInfoLog( program, len, &len, log );
//            std::cerr << "Shader linking failed: " << log << std::endl;
//            delete [] log;
//#endif /* DEBUG */
//            
//            for ( entry = shaders; entry->type != GL_NONE; ++entry ) {
//                glDeleteShader( entry->shader );
//                entry->shader = 0;
//            }
//            
//            return 0;
//        }
//        
//        return program;
//    }
//    
//    //----------------------------------------------------------------------------
//#ifdef __cplusplus
//}
//#endif // __cplusplus




















