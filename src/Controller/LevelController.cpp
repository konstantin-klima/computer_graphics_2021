//
// Created by konstantin on 03/02/2021.
//

#include "LevelController.h"
#include "../Entity/EntityManager.h"
#include "../Entity/ModelManager.h"
#include "../Entity/ShaderManager.h"
#include "../constants.h"

void LevelController::init(rp3d::PhysicsCommon *physicsCommon, rp3d::PhysicsWorld *world) {
    // Arena setup
    auto arena = new Entity();
    arena->addComponent<ModelComponent>(ModelManager::getManager().getModel("arena"));
    arena->addComponent<RigidBodyComponent>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, world, true);
    arena->addComponent<ConcaveColliderComponent>(arena->getComponent<ModelComponent>()->getMeshes(), physicsCommon);
    arena->getComponent<RigidBodyComponent>()->addCollider(arena->getComponent<ConcaveColliderComponent>()->getShape());
    auto arenaShader = ShaderComponent(ShaderManager::getManager().getShader("basic"));
    arena->addComponent<ShaderComponent>(arenaShader);
    arena->getComponent<RigidBodyComponent>()->getRigidBody()->setUserData((void *)&"ARENA");
    EntityManager::getManager().addEntity(arena);


    auto skybox = new Entity();
    skybox->addComponent<SkyboxComponent>();
    auto skyboxShader = ShaderComponent(ShaderManager::getManager().getShader("skybox"));
    skybox->addComponent<ShaderComponent>(skyboxShader);
    EntityManager::getManager().addEntity(skybox);

    auto light = new Entity();
    auto lc = LightComponent(
            glm::vec3(4.0f, 4.0, 0.0),
            glm::vec3(5, 5, 5),
            glm::vec3(0.9, 0.9, 0.9),
            glm::vec3(1.0, 1.0, 1.0),
            1.0f,
            0.09f,
            0.032f
    );
    light->addComponent<LightComponent>(lc);

    EntityManager::getManager().addEntity(light);
}

void LevelController::update() {

}
