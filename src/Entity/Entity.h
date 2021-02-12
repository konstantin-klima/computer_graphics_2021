//
// Created by konstantin on 02/02/2021.
//

#ifndef PROJECT_BASE_ENTITY_H
#define PROJECT_BASE_ENTITY_H
#include <vector>
#include <algorithm>
#include <memory>
#include "Component.h"

class Entity {
    inline static unsigned cID = 0;
    unsigned m_ID;
    std::vector<std::unique_ptr<Component>> m_Comps {};

public:
    Entity() { m_ID = cID++; }

    void addComponent(Component* component);

    template<typename T>
    T* getComponent() {
        auto id = Component::GetComponentTypeID<T>();
        auto res = std::find_if(m_Comps.begin(), m_Comps.end(), [id](std::unique_ptr<Component>& c){return c->m_TypeId == id;});
        if(res != m_Comps.end())
            return static_cast<T*>((*res).get());
        else
            return nullptr;
    };

    template<typename T>
    bool hasComponent() {
        if(m_Comps.size() == 0)
            return false;
        auto id = Component::GetComponentTypeID<T>();
        auto res = std::find_if(m_Comps.begin(), m_Comps.end(), [id](std::unique_ptr<Component>& c){return c->m_TypeId == id;});

        return res != m_Comps.end();
    };

    template <typename TComp, typename... TArgs>
    TComp& addComponent(TArgs&&... _mArgs)
    {
        auto component = std::make_unique<TComp>(std::forward<TArgs>(_mArgs)...);
        component->m_TypeId = Component::GetComponentTypeID<TComp>();
        m_Comps.push_back(std::move(component));

        return *(static_cast<TComp*>(m_Comps.back().get()));
    }

    template<typename T>
    void removeComponent() {
        if(m_Comps.size() == 0)
            return;

        auto id = Component::GetComponentTypeID<T>();
        auto res = std::find_if(m_Comps.begin(), m_Comps.end(), [id](std::unique_ptr<Component>& c){return c->m_TypeId == id;});

        if(res != m_Comps.end()){
            m_Comps.erase(res);
        }
    };

    [[nodiscard]]
    unsigned getID() const {
        return m_ID;
    }
};


#endif //PROJECT_BASE_ENTITY_H
