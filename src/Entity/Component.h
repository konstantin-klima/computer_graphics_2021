//
// Created by konstantin on 02/02/2021.
//

#ifndef PROJECT_BASE_COMPONENT_H
#define PROJECT_BASE_COMPONENT_H


#include <glm/vec3.hpp>

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

enum LIGHTS {
    SPECULAR,
    SPOTLIGHT
};

struct LightComponent: public Component {
    LIGHTS type {SPECULAR};
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 direction;
    float cutOff;
    float cutOffOuter;

    LightComponent(glm::vec3&& position, glm::vec3&& ambient, glm::vec3&& defuse, glm::vec3&& specular, float constant, float linear, float quadratic)
    :position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant), linear(linear), quadratic(quadratic) {};
};

#endif //PROJECT_BASE_COMPONENT_H
