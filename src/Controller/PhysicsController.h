//
// Created by rentorious on 03/02/2021.
//

#ifndef PROJECT_BASE_PHYSICSCONTROLLER_H
#define PROJECT_BASE_PHYSICSCONTROLLER_H

#include "reactphysics3d/reactphysics3d.h"
#include "EventListener.h"
#include <chrono>

struct PhysicsController {
	static void init();

	static void update();

	static rp3d::PhysicsCommon* getPhysicsCommon() { return physicsCommon; }

	static rp3d::PhysicsWorld* getWorld() { return world; }

	static double getDeltaTime() { return deltaTime.count(); }

  private:
	inline static bool initialized = false;
	// zasto mora inline???
	inline static const float timeStep = 1 / 60.0f;
	inline static rp3d::PhysicsCommon* physicsCommon;
	inline static rp3d::PhysicsWorld* world;
	inline static std::chrono::system_clock::time_point previousTime;
	inline static std::chrono::system_clock::time_point currentTime;
	inline static long double accumulator;
	inline static std::chrono::duration<double> deltaTime;
	inline static EventListener* eventListener;
};

#endif // PROJECT_BASE_PHYSICSCONTROLLER_H
