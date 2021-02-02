//
// Created by konstantin on 02/02/2021.
//

#ifndef PROJECT_BASE_COMPONENT_H
#define PROJECT_BASE_COMPONENT_H


// Base component code adapted from Nikola Sobajic
typedef unsigned ComponentTypeID;
struct Component
{
    ComponentTypeID m_TypeId{ 0 };
    bool m_Active{ false };
    virtual ~Component() = default;

private:
    inline static ComponentTypeID m_MaxComponentTypeId = 0;

public:
    template <typename T>
    static ComponentTypeID GetComponentTypeID()
    {
        static_assert (std::is_base_of<Component, T>::value, "");
        static ComponentTypeID typeID = Component::m_MaxComponentTypeId++;
        return typeID;
    }
};


#endif //PROJECT_BASE_COMPONENT_H
