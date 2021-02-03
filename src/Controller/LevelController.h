//
// Created by konstantin on 03/02/2021.
//

#ifndef PROJECT_BASE_LEVELCONTROLLER_H
#define PROJECT_BASE_LEVELCONTROLLER_H


#include <reactphysics3d/engine/PhysicsWorld.h>

struct LevelController {
    static void init(reactphysics3d::PhysicsWorld *world);
    static void update();
};


#endif //PROJECT_BASE_LEVELCONTROLLER_H
