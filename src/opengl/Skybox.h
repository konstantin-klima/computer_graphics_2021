//
// Created by konstantin on 04/02/2021.
//

#ifndef PROJECT_BASE_SKYBOX_H
#define PROJECT_BASE_SKYBOX_H


#include <learnopengl/model.h>

class Skybox: public Model{
    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
};


#endif //PROJECT_BASE_SKYBOX_H
