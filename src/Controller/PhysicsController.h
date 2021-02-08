//
// Created by rentorious on 03/02/2021.
//

#ifndef PROJECT_BASE_PHYSICSCONTROLLER_H
#define PROJECT_BASE_PHYSICSCONTROLLER_H

#include "reactphysics3d/reactphysics3d.h"

struct PhysicsController {
    static void init();

    static void update();



    static rp3d::PhysicsCommon *getPhysicsCommon() {
        return physicsCommon;
    }

    static rp3d::PhysicsWorld *getWorld() {
        return world;
    }

private:
    inline static bool initialized = false;
    // zasto mora inline???
    inline static const float timeStep = 1 / 60.0f;
    inline static rp3d::PhysicsCommon *physicsCommon;
    inline static rp3d::PhysicsWorld *world;
    inline static long double previousTime;
    inline static long double accumulator;
};

#endif //PROJECT_BASE_PHYSICSCONTROLLER_H
