//
// Created by konstantin on 03/02/2021.
//

#ifndef PROJECT_BASE_LEVELCONTROLLER_H
#define PROJECT_BASE_LEVELCONTROLLER_H


#include "reactphysics3d/reactphysics3d.h"

struct LevelController {
    static void init(rp3d::PhysicsWorld *world, rp3d::PhysicsCommon *physicsCommon);
    static void update();
};


#endif //PROJECT_BASE_LEVELCONTROLLER_H
