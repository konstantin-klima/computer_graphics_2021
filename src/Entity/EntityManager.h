//
// Created by konstantin on 02/02/2021.
//
#ifndef PROJECT_BASE_ENTITYMANAGER_H
#define PROJECT_BASE_ENTITYMANAGER_H

#include <vector>
#include <algorithm>
#include "Entity.h"


struct EntityManager {

    static EntityManager& getManager(){
        static EntityManager instance;
        return instance;
    }

    EntityManager(EntityManager const&) = delete;
    void operator=(EntityManager const&) = delete;

    void addEntity(Entity* e)
    {
        m_entities.emplace_back(e);
    }
    void addEntity(std::unique_ptr<Entity>&& e)
    {
        m_entities.push_back(std::move(e));
    }

    template<typename T>
    std::vector<Entity*> getEntitiesWithComponent(){
        std::vector<Entity*> res;

        for(const auto& e : m_entities){
            if(e->hasComponent<T>())
                res.push_back(e.get());
        }
        return res;
    }

    template<typename T>
    std::vector<T*> getAllComponents(){
        std::vector<T*> res;

        for(const auto& e : m_entities){
            if(e->hasComponent<T>())
                res.push_back(e->getComponent<T>());
        }
        return res;
    }

    void clearEntities(){
        m_entities.clear();
    }

private:
    std::vector<std::unique_ptr<Entity>> m_entities {};
    EntityManager() = default;
};


#endif //PROJECT_BASE_ENTITYMANAGER_H
