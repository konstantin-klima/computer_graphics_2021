//
// Created by konstantin on 03/02/2021.
//

#ifndef PROJECT_BASE_PLAYERCONTROLLER_H
#define PROJECT_BASE_PLAYERCONTROLLER_H

#ifndef GLFW_HEAD_H
	#define GLFW_HEAD_H
	#include <glad/glad.h>
	#include <GLFW/glfw3.h>

	#include "reactphysics3d/reactphysics3d.h"
	#include "../constants.h"

#endif

#include "SpellFactory.h"

struct Entity;

struct PlayerController {
	inline static bool initialized = false;

	static void init(rp3d::PhysicsCommon* physicsCommon,
					 rp3d::PhysicsWorld* world);
	static void update();
	static void processInput(GLFWwindow* window);

	static void processMouse(float xoffset, float yoffset);

  private:
	static void castSpell(Entity* player, SPELL spell);
	static void switchFlashlight(Entity* player);
	static void updateFlashlightPosition(Entity* player);

	inline static unsigned p1_lastKeyState_1 = 0;
	inline static unsigned p1_lastKeyState_2 = 0;
	inline static unsigned p1_lastKeyState_3 = 0;
	inline static unsigned p1_lastKeyState_4 = 0;
	inline static unsigned p1_lastKeyState_f = 0;
};

#endif // PROJECT_BASE_PLAYERCONTROLLER_H
