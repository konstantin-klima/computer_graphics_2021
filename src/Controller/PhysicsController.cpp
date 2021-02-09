//
// Created by rentorious on 03/02/2021.
//

//
// Created by konstantin on 03/02/2021.
//

#include "PhysicsController.h"

#include <iostream>

void PhysicsController::init() {
    if (initialized) {
        return;
    }

    physicsCommon = new rp3d::PhysicsCommon;
    world = physicsCommon->createPhysicsWorld();
    previousTime = std::chrono::system_clock::now();
    currentTime = std::chrono::system_clock::now();
    accumulator = 0;

    eventListener = new EventListener();

    world->setEventListener(eventListener);

    deltaTime = currentTime - previousTime;

    initialized = true;
}

void PhysicsController::update() {
    currentTime = std::chrono::system_clock::now();
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;

    accumulator += deltaTime.count();

    while (accumulator >= timeStep) {
        world->update(timeStep);
        accumulator -= timeStep;
    }

    // TODO: add interpolation

//    float factor = accumulator / timeStep;

}
