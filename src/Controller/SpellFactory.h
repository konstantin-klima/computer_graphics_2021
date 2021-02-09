//
// Created by konstantin on 08/02/2021.
//

#ifndef PROJECT_BASE_SPELLFACTORY_H
#define PROJECT_BASE_SPELLFACTORY_H

struct Entity;
struct Camera;

enum SPELL {
    FIREBALL,
    BOLT
};

class SpellFactory {
    Entity* m_player;
    SPELL m_spell;
    Entity* m_spellEntity;
    Camera* m_camera;


    void makeLightComponent();
    void makeMovementComponent();
    void makeSpellPropertyComponent();

public:
    SpellFactory(Entity* player, SPELL spell);
    Entity* makeSpell();
};


#endif //PROJECT_BASE_SPELLFACTORY_H
