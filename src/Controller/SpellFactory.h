//
// Created by konstantin on 08/02/2021.
//

#ifndef PROJECT_BASE_SPELLFACTORY_H
#define PROJECT_BASE_SPELLFACTORY_H

struct Entity;
struct Camera;

enum SPELL {
    VOID_BOLT,
    SPECTRAL_ARROW,
    FROST_BOMB
};

class SpellFactory {
    Entity* m_player;
    SPELL m_spell;
    Entity* m_spellEntity;
    Camera* m_camera;


    void makeLightComponent();
    void makeMovementComponent();
    void makeSpellPropertyComponent();
    void makeShaderComponent();
    void makeModelComponent();
    void makeRigidBodyComponent();
    void makeColliderComponent();

public:
    SpellFactory(Entity* player, SPELL spell);
    Entity* makeSpell();
};


#endif //PROJECT_BASE_SPELLFACTORY_H
