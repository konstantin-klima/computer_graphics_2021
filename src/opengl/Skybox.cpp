//
// Created by konstantin on 04/02/2021.
//

#include "Skybox.h"
#include "../include/common.h"

unsigned int Skybox::TextureFromFile(const char *path, const string &directory, bool gamma) {

    unsigned textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


    return textureID;
}
