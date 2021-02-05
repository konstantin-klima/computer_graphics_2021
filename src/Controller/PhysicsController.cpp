//
// Created by rentorious on 03/02/2021.
//

//
// Created by konstantin on 03/02/2021.
//

#include <GLFW/glfw3.h>

#include "PhysicsController.h"
#include "../Entity/EntityManager.h"

#include "reactphysics3d/reactphysics3d.h"

void PhysicsController::init() {
    if (initialized) {
        return;
    }

    physicsCommon = new rp3d::PhysicsCommon;
    world = physicsCommon->createPhysicsWorld();
    previousTime = 0;
    accumulator = 0;

    initialized = true;
}

void PhysicsController::update() {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - previousTime;
    previousTime = currentTime;

    accumulator += deltaTime;


    while (accumulator >= timeStep) {
        world->update(timeStep);
        accumulator -= timeStep;
    }

    float factor = accumulator / timeStep;

    // TODO: add interpolation
}
