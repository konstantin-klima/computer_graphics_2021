//
// Created by konstantin on 03/02/2021.
//

#include "PlayerController.h"

#include "PhysicsController.h"
#include "../Entity/EntityManager.h"

void PlayerController::init(rp3d::PhysicsCommon *physicsCommon, rp3d::PhysicsWorld *world) {
    if (initialized) {
        return;
    }

    auto player1 = new Entity();
    player1->addComponent<CameraComponent>(10, 10, 0, 0);
    player1->addComponent<MovementComponent>(0, 0, 0, 20.0f);
    player1->addComponent<BoxColliderComponent>(0.3, 1, 0.3, physicsCommon);
    player1->addComponent<RigidBodyComponent>(10, 20, 0, 0, -99.0f, world);
    auto p1Body = player1->getComponent<RigidBodyComponent>();
    p1Body->setMass(100.0f);
    p1Body->addCollider(player1->getComponent<BoxColliderComponent>()->getShape());
    //p1Body->getRigidBody()->getCollider(0)->getMaterial().setFrictionCoefficient(0.0f);
    EntityManager::getManager().addEntity(player1);

    auto player2 = new Entity();
    player2->addComponent<CameraComponent>(-10, 10, 0, 1);
    player2->addComponent<MovementComponent>(0, 0, 0, 20.0f);
    player2->addComponent<BoxColliderComponent>(0.3, 1, 0.3, physicsCommon);
    player2->addComponent<RigidBodyComponent>(-20, 20, 0, 0, -99.0f, world);
    auto p2Body = player2->getComponent<RigidBodyComponent>();
    p2Body->setMass(100.0f);
    p2Body->addCollider(player2->getComponent<BoxColliderComponent>()->getShape());
    EntityManager::getManager().addEntity(player2);

    initialized = true;
}

void PlayerController::update() {
    auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();

    for (auto player : players) {
        auto body = player->getComponent<RigidBodyComponent>();
        auto movement = player->getComponent<MovementComponent>();
        auto camera = player->getComponent<CameraComponent>();

        auto dir = movement->getDirection();
        auto speed = movement->getSpeed()*10;

        body->applyForceAtCenter(dir * speed);
        camera->setCameraPos(body->getGLMPosition());

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
            auto direction = rp3d::Vector3(0, 0, 0);

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                direction.z += -1;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                direction.z += 1;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                direction.x += -1;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                direction.x += 1;

            auto p1_currentKeyState_1 = glfwGetKey(window, GLFW_KEY_1);
            if (p1_currentKeyState_1 == GLFW_RELEASE && p1_lastKeyState_1 == GLFW_PRESS)
                castSpell(player, SPELL::FIREBALL);
            p1_lastKeyState_1 = p1_currentKeyState_1;

            direction.normalize();
            auto movement = player->getComponent<MovementComponent>();
            movement->setDirection(&direction);
        } else {
            auto direction = rp3d::Vector3(0, 0, 0);

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                direction.z += -1;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                direction.z += 1;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                direction.x += -1;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                direction.x += 1;

            direction.normalize();
            auto movement = player->getComponent<MovementComponent>();
            movement->setDirection(&direction);
        }
    }
}

void PlayerController::processMouse(float xoffset, float yoffset) {
    float MouseSensitivity = 1.0;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();
    for(auto player : players) {
        auto camera = player->getComponent<CameraComponent>();
        if (camera->camIndex == 0) {
            camera->updateCameraVectors(xoffset, yoffset);
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
    std::cout << EntityManager::getManager().getAllComponents<LightComponent>().size() << std::endl;
}