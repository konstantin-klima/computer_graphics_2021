//
// Created by konstantin on 03/02/2021.
//

#include "PlayerController.h"
#include "../Entity/EntityManager.h"

void PlayerController::init() {
    if(initialized){
        return;
    }

    auto player1 = new Entity();
    player1->addComponent<CameraComponent>(10, 10, 0, 0);
    EntityManager::getManager().addEntity(player1);

    auto player2 = new Entity();
    player2->addComponent<CameraComponent>(-20, 10, 0, 1);
    EntityManager::getManager().addEntity(player2);

    initialized = true;
}

void PlayerController::update() {
    auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();

}

void PlayerController::processInput(GLFWwindow *window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
        return;
    }


    auto cameras = EntityManager::getManager().getAllComponents<CameraComponent>();
    for(auto camera : cameras){
        if(camera->camIndex == 0){
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(FORWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(BACKWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(LEFT, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        else{
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(FORWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(BACKWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(LEFT, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(RIGHT, deltaTime);
        }
    }



}