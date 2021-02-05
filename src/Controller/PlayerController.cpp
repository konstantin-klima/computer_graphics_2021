//
// Created by konstantin on 03/02/2021.
//

#include "PlayerController.h"
#include "../Entity/EntityManager.h"
#include "PhysicsController.h"

void PlayerController::init(rp3d::PhysicsWorld *world) {
    if(initialized){
        return;
    }

    auto player1 = new Entity();
    player1->addComponent<CameraComponent>(10, 10, 0, 0);
    player1->addComponent<VelocityComponent>(0, 0, 0);
    player1->addComponent<RigidBodyComponent>(10, 0, 0, 0, -99.0f, world);
    EntityManager::getManager().addEntity(player1);

    auto player2 = new Entity();
    player2->addComponent<CameraComponent>(-20, 10, 0, 1);
    player2->addComponent<VelocityComponent>(0, 0, 00);
    player1->addComponent<RigidBodyComponent>(-20, 0, 0, 0, -99.0f, world);
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


    auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();
    for(auto player : players){
        auto body = player->getComponent<RigidBodyComponent>()
        if(camera->camIndex == 0) {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(BACKWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(LEFT, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera->camera.ProcessKeyboard(RIGHT, deltaTime);

    }



}