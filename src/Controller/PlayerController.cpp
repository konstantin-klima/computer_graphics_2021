//
// Created by konstantin on 03/02/2021.
//

#include "PlayerController.h"
#include "../Entity/EntityManager.h"
#include "PhysicsController.h"
#include "../constants.h"
#include "../Entity/ShaderManager.h"
#include <iostream>

void PlayerController::init(rp3d::PhysicsCommon *physicsCommon, rp3d::PhysicsWorld *world) {
    if (initialized) {
        return;
    }

    auto player1 = new Entity();
    player1->addComponent<CameraComponent>(10, 20, 0, 0);
    player1->addComponent<MovementComponent>(0, 0, 0, 15.0f);
    player1->addComponent<CapsuleColliderComponent>(0.5, 1.0, physicsCommon);
    player1->addComponent<CollisionBodyComponent>(10, 10, 0, world);
    player1->addComponent<PlayerModelComponent>();
    player1->addComponent<ShaderComponent>(ShaderManager::getManager().getShader("player"));
    player1->addComponent<HealthComponent>(1000);
    auto p1Body = player1->getComponent<CollisionBodyComponent>();
    p1Body->getBody()->setUserData((void *) &"PLAYER1");
    p1Body->addCollider(player1->getComponent<CapsuleColliderComponent>()->getShape());
    p1Body->getBody()->getCollider(0)->setCollisionCategoryBits(Settings::CollisionCategory::PLAYER1 | Settings::CollisionCategory::PLAYER);
    EntityManager::getManager().addEntity(player1);

    auto player2 = new Entity();
    player2->addComponent<CameraComponent>(-10, 20, -10, 1);
    player2->addComponent<MovementComponent>(0, 0, 0, 15.0f);
    player2->addComponent<CapsuleColliderComponent>(0.5, 1.01, physicsCommon);
    player2->addComponent<CollisionBodyComponent>(-10, 10, -10, world);
    player2->addComponent<PlayerModelComponent>();
    player2->addComponent<ShaderComponent>(ShaderManager::getManager().getShader("player"));
    player2->addComponent<HealthComponent>(1000);
    auto p2Body = player2->getComponent<CollisionBodyComponent>();
    p2Body->getBody()->setUserData((void *) &"PLAYER2");
    p2Body->addCollider(player2->getComponent<CapsuleColliderComponent>()->getShape());
    p2Body->getBody()->getCollider(0)->setCollisionCategoryBits(Settings::CollisionCategory::PLAYER2 | Settings::CollisionCategory::PLAYER);
    EntityManager::getManager().addEntity(player2);

    initialized = true;
}

void PlayerController::update() {
    auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();

    for (auto player : players) {
        // Cant move if u ded
        if (player->getComponent<HealthComponent>()->getCurrentHealth() <= 0)
            continue;
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
        auto forward = player->getComponent<CameraComponent>()->getRP3DFront();
        forward.y = 0;
        auto right = player->getComponent<CameraComponent>()->getRP3DRight();
        right.y = 0;

        if (player->getComponent<CameraComponent>()->camIndex == 0) {
            auto direction = rp3d::Vector3(0, 0, 0);

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                direction += forward;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                direction += -forward;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                direction += -right;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                direction += right;

            auto p1_currentKeyState_f = glfwGetKey(window, GLFW_KEY_F);
            if (p1_currentKeyState_f == GLFW_RELEASE && p1_lastKeyState_f == GLFW_PRESS) {
                switchFlashlight(player);
            }
            p1_lastKeyState_f = p1_currentKeyState_f;

            auto p1_currentKeyState_1 = glfwGetKey(window, GLFW_KEY_1);
            if (p1_currentKeyState_1 == GLFW_RELEASE && p1_lastKeyState_1 == GLFW_PRESS)
                castSpell(player, SPELL::FROST_BOMB);
            p1_lastKeyState_1 = p1_currentKeyState_1;

            direction.normalize();
            auto movement = player->getComponent<MovementComponent>();
            movement->setDirection(&direction);

            updateFlashlightPosition(player);
        } else {
            auto direction = rp3d::Vector3(0, 0, 0);

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                direction += forward;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                direction += -forward;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                direction += -right;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                direction += right;

            direction.normalize();
            auto movement = player->getComponent<MovementComponent>();
            movement->setDirection(&direction);
        }
    }
}

void PlayerController::updateFlashlightPosition(Entity* player){
    if(player->hasComponent<LightComponent>()){
        auto camera = player->getComponent<CameraComponent>()->camera;
        auto light = player->getComponent<LightComponent>();

        light->position = camera.Position;
        light->direction = camera.Front;
    }
}

void PlayerController::processMouse(float xoffset, float yoffset) {
    float MouseSensitivity = 1.0;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();
    for (auto player : players) {
        auto camera = player->getComponent<CameraComponent>();
        if (camera->camIndex == 0) {
            camera->updateCameraVectors(xoffset, yoffset);
            updateFlashlightPosition(player);
        }
    }

}


void PlayerController::castSpell(Entity *player, SPELL spell) {
    // Do I have the spell inside my inventory?
    // Is there an active cooldown?
    // Which spell will I be casting?

    // When these questions are answered call the SpellFactory.

    // Make an Entity. It's inside EntityManager, the pointer is here for any additional changes
    auto spellEntity = SpellFactory(player, spell).makeSpell();
}

void PlayerController::switchFlashlight(Entity *player){
    if(!player->hasComponent<LightComponent>()){
        auto camera = player->getComponent<CameraComponent>()->camera;
        auto lc = LightComponent(
                std::forward<glm::vec3>(camera.Position),
                std::forward<glm::vec3>(camera.Front),
                glm::vec3(1.f, 1.0f, 0.f),
                glm::vec3(0, 1.f, 0.f),
                glm::vec3(0, 1.f, 0.f),
                glm::cos(glm::radians(12.5f)),
                glm::cos(glm::radians(15.0f)),
                1.0f,
                0.09f,
                0.032f
        );
        player->addComponent<LightComponent>(lc);
    }
    else{
        player->removeComponent<LightComponent>();
    }

}