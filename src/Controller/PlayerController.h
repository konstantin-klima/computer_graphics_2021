//
// Created by konstantin on 03/02/2021.
//

#ifndef PROJECT_BASE_PLAYERCONTROLLER_H
#define PROJECT_BASE_PLAYERCONTROLLER_H

#ifndef GLFW_HEAD_H
#define GLFW_HEAD_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#include "../Entity/EntityManager.h"

struct PlayerController {
    inline static bool initialized = false;

    static void init();
    static void update();
    static void processInput(GLFWwindow *window, float deltaTime);
};


#endif //PROJECT_BASE_PLAYERCONTROLLER_H
