//
// Created by konstantin on 03/02/2021.
//

#include "PlayerController.h"
#include "../Entity/EntityManager.h"
#include "PhysicsController.h"
#include "../constants.h"

#include <iostream>

void PlayerController::init(rp3d::PhysicsCommon *physicsCommon, rp3d::PhysicsWorld *world) {
    if (initialized) {
        return;
    }

    auto player1 = new Entity();
    player1->addComponent<CameraComponent>(10, 10, 0, 0);
    player1->addComponent<MovementComponent>(0, 0, 0, 15.0f);
    player1->addComponent<CapsuleColliderComponent>(0.3, 1.0, physicsCommon);
    player1->addComponent<CollisionBodyComponent>(10, 20, 0, world);
    auto p1Body = player1->getComponent<CollisionBodyComponent>();
    p1Body->getBody()->setUserData((void *)&"PLAYER1");
    p1Body->addCollider(player1->getComponent<CapsuleColliderComponent>()->getShape());
    EntityManager::getManager().addEntity(player1);


    auto player2 = new Entity();
    player2->addComponent<CameraComponent>(-10, 10, -10, 1);
    player2->addComponent<MovementComponent>(0, 0, 0, 15.0f);
    player2->addComponent<CapsuleColliderComponent>(0.3, 1.0, physicsCommon);
    player2->addComponent<CollisionBodyComponent>(-10, 10, -10, world);
    auto p2Body = player2->getComponent<CollisionBodyComponent>();
    p2Body->getBody()->setUserData((void *)&"PLAYER2");
    p2Body->addCollider(player2->getComponent<CapsuleColliderComponent>()->getShape());

    EntityManager::getManager().addEntity(player2);

    initialized = true;
}

void PlayerController::update() {
    auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();

    for (auto player : players) {
        auto body = player->getComponent<CollisionBodyComponent>();
        player->getComponent<MovementComponent>()->moveCollisionBody(body, PhysicsController::getDeltaTime());
        auto newPos = body->getPosition();
        player->getComponent<CameraComponent>()->setCameraPos(glm::vec3(newPos.x, newPos.y, newPos.z));
    }

}

void PlayerController::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }


    auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();
    for (auto player : players) {
        if (player->getComponent<CameraComponent>()->camIndex == 0) {
            auto direction = new rp3d::Vector3(0, 0, 0);

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                (*direction).z += -1;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                (*direction).z += 1;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                (*direction).x += -1;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                (*direction).x += 1;

            direction->normalize();
            auto movement = player->getComponent<MovementComponent>();
            movement->setDirection(direction);
        } else {
            auto direction = new rp3d::Vector3(0, 0, 0);

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                (*direction).z += -1;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                (*direction).z += 1;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                (*direction).x += -1;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                (*direction).x += 1;

            direction->normalize();
            auto movement = player->getComponent<MovementComponent>();
            movement->setDirection(direction);
        }
    }


}