//
// Created by konstantin on 03/02/2021.
//

#include "LevelController.h"
#include "../Entity/EntityManager.h"
#include "../Entity/ModelManager.h"
#include "../Entity/ShaderManager.h"

void LevelController::init(reactphysics3d::PhysicsWorld *world) {

    auto arenaModel = ModelManager::getManager().getModel("arena");

    auto arena = new Entity();
    arena->addComponent<ModelComponent>(arenaModel);
    auto arenaShader = ShaderComponent();
    arenaShader.addShader("basic", ShaderManager::getManager().getShader("basic"));
    arena->addComponent<ShaderComponent>(arenaShader);
    EntityManager::getManager().addEntity(arena);


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

      reactphysics3d::PhysicsCommon physicsCommon;
      // ReactPhysics3D HelloWorld


      reactphysics3d::Vector3 position(0, 10, 0);
      reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
      reactphysics3d::Transform transform(position, orientation);
      reactphysics3d::RigidBody *body = world->createRigidBody(transform);
      rp3d::Vector3 halfExtents(0.5, 0.75, 0.5);

      // TODO Ovde puca nesto - izgleda da je do alokacije kolajdera
      /*
      auto colliderShape = physicsCommon.createCapsuleShape(0.5, 1.6);
      body->addCollider(colliderShape, rp3d::Transform::identity());
        */
      // arena
      auto arenaTransform = rp3d::Transform::identity();
      rp3d::RigidBody *arenaBody = world->createRigidBody(arenaTransform);
      arenaBody->setType(rp3d::BodyType::STATIC);
      arenaBody->addCollider(arenaModel->concaveCollider->collider, arenaTransform);
      arenaBody->enableGravity(false);
}

void LevelController::update() {

}
