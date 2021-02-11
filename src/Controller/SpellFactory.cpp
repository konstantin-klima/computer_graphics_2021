//
// Created by konstantin on 08/02/2021.
//

#include "SpellFactory.h"
#include "../Entity/Component.h"
#include "../Entity/EntityManager.h"
#include "learnopengl/model.h"
#include "../Entity/ModelManager.h"
#include "../Entity/ShaderManager.h"
#include "PhysicsController.h"

SpellFactory::SpellFactory(Entity *player, SPELL spell)
    : m_player(player)
    , m_spell(spell)
    {
        m_spellEntity = new Entity();
        m_camera = &player->getComponent<CameraComponent>()->camera;
    }

Entity *SpellFactory::makeSpell() {
    // Some components are referencing others so the order of these calls is important
    makeLightComponent();
    makeMovementComponent();
    makeShaderComponent();
    makeModelComponent();
    makeSpellPropertyComponent();
    makeColliderComponent();
    makeRigidBodyComponent();

    EntityManager::getManager().addEntity(m_spellEntity);
    return m_spellEntity;
}

void SpellFactory::makeLightComponent() {
    if(m_spell == VOID_BOLT){
        auto lc = LightComponent(
                std::forward<glm::vec3>(m_camera->Position),
                glm::vec3(5, 1, 1),
                glm::vec3(2.4, 2.3, 1.4),
                glm::vec3(1.0, 1.0, 1.0),
                1.0f,
                0.09f,
                0.032f
        );
        m_spellEntity->addComponent<LightComponent>(lc);
    } else if (m_spell == SPECTRAL_ARROW) {
        auto lc = LightComponent(
                std::forward<glm::vec3>(m_camera->Position),
                glm::vec3(2, 2, 3),
                glm::vec3(5.4, 1.3, 6.4),
                glm::vec3(1.0, 1.0, 1.0),
                1.0f,
                0.09f,
                0.032f
        );
        m_spellEntity->addComponent<LightComponent>(lc);
    } else if (m_spell == FROST_BOMB) {
        auto lc = LightComponent(
                std::forward<glm::vec3>(m_camera->Position),
                glm::vec3(1, 1, 3),
                glm::vec3(0.1, 1.3, 6.4),
                glm::vec3(1.0, 1.0, 1.0),
                1.0f,
                0.09f,
                0.032f
        );
        m_spellEntity->addComponent<LightComponent>(lc);
    }
}

void SpellFactory::makeMovementComponent() {
    auto direction = m_camera->Front;
    if(m_spell == VOID_BOLT || m_spell == SPECTRAL_ARROW || m_spell == FROST_BOMB) {
        auto mc = MovementComponent(
                direction.x,
                direction.y,
                direction.z,
                0
                );
        m_spellEntity->addComponent<MovementComponent>(mc);
    }
}

void SpellFactory::makeShaderComponent() {
    auto shader = ShaderComponent(ShaderManager::getManager().getShader("basic"));
    m_spellEntity->addComponent<ShaderComponent>(shader);
}

void SpellFactory::makeModelComponent() {
    if(m_spell == VOID_BOLT) {
        auto model = ModelManager::getManager().getModel("voidBolt");
        auto mc = ModelComponent(model);
        m_spellEntity->addComponent<ModelComponent>(mc);
    } else if (m_spell == SPECTRAL_ARROW) {
        auto model = ModelManager::getManager().getModel("spectralArrow");
        auto mc = ModelComponent(model);
        m_spellEntity->addComponent<ModelComponent>(mc);
    } else if (m_spell == FROST_BOMB) {
        auto model = ModelManager::getManager().getModel("frostBomb");
        auto mc = ModelComponent(model);
        m_spellEntity->addComponent<ModelComponent>(mc);
    }
}

void SpellFactory::makeColliderComponent() {
    auto cc = SphereColliderComponent(0.5f, PhysicsController::getPhysicsCommon());
    m_spellEntity->addComponent<SphereColliderComponent>(cc);
}

void SpellFactory::makeRigidBodyComponent() {
    auto position = m_camera->Position;
    auto front = m_camera->Front;
    auto body = RigidBodyComponent(position.x + front.x, position.y + front.y, position.z + front.z, 0, 0, PhysicsController::getWorld());
    body.getRigidBody()->enableGravity(false);
    auto linearVelocity = m_spellEntity->getComponent<MovementComponent>()->getDirection() * m_spellEntity->getComponent<MovementComponent>()->getSpeed();
    body.getRigidBody()->setLinearVelocity(linearVelocity);
    body.addCollider(m_spellEntity->getComponent<SphereColliderComponent>()->getShape());
    m_spellEntity->addComponent<RigidBodyComponent>(body);

}

void SpellFactory::makeSpellPropertyComponent() {
    // This is just a placeholder right now
    auto sp = SpellPropertyComponent(100, 100, SPELL_TYPES::BULLET);
    m_spellEntity->addComponent<SpellPropertyComponent>(sp);
}
