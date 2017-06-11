//
//  Skybox.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/10/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "Skybox.hpp"

using namespace std;

Skybox::Skybox(vector<const char *> cubeFaces) {
    
    glGenTextures(1, &cubeMapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
    
    for(GLuint i = 0; i < cubeFaces.size(); i++) {
        NS_TGALOADER::IMAGE image;
        image.LoadTGA(cubeFaces[i]);
        
        cout << "image " << i << ", " << image.getWidth() << ", " << image.getHeight() << endl;
        
        if(image.getDataForOpenGL() == 0) cout << "ERROR: image data not loaded correctly" << endl;
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_BGRA,
                     image.getWidth(), image.getHeight(), 0,
                     GL_BGRA, GL_UNSIGNED_BYTE, image.getDataForOpenGL());
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::genCube() {
    cout << "genCube" << endl;
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);
    
    glGenBuffers(1, &skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
}
