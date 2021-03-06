//
// Created by konstantin on 03/02/2021.
//

#ifndef PROJECT_BASE_RENDERCONTROLLER_H
#define PROJECT_BASE_RENDERCONTROLLER_H

#include "../include/learnopengl/shader.h"
#include "../Entity/ShaderManager.h"
#include "../Entity/EntityManager.h"
#include "../constants.h"

class RenderController {


public:
    static void init();
    static void render();

    static void loadShaders();
    static void updateLights();
    static void drawEntities();

    static void clearGlBuffers();

    static void drawOtherPlayer(Entity *player);
private:
    static void updateShaderView(Camera c);
};


#endif //PROJECT_BASE_RENDERCONTROLLER_H
