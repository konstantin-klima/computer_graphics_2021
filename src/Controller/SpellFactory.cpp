//
// Created by konstantin on 08/02/2021.
//

#include "SpellFactory.h"
#include "../Entity/Component.h"
#include "../Entity/EntityManager.h"

SpellFactory::SpellFactory(Entity *player, SPELL spell)
    : m_player(player)
    , m_spell(spell)
    {
        m_spellEntity = new Entity();
        m_camera = &player->getComponent<CameraComponent>()->camera;
    }

Entity *SpellFactory::makeSpell() {
    makeLightComponent();
    makeMovementComponent();
    makeSpellPropertyComponent();

    EntityManager::getManager().addEntity(m_spellEntity);
    return m_spellEntity;
}

void SpellFactory::makeLightComponent() {
    if(m_spell == FIREBALL){
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
    }
}

void SpellFactory::makeMovementComponent() {
    auto direction = m_camera->Front;
    if(m_spell == FIREBALL) {
        auto mc = MovementComponent(
                direction.x,
                direction.y,
                direction.z,
                30
                );
        m_spellEntity->addComponent<MovementComponent>(mc);
    }
}

void SpellFactory::makeSpellPropertyComponent() {

}
