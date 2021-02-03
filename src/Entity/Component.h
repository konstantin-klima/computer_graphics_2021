//
// Created by konstantin on 02/02/2021.
//

#ifndef PROJECT_BASE_COMPONENT_H
#define PROJECT_BASE_COMPONENT_H


#include <glm/vec3.hpp>
#include <learnopengl/camera.h>

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

    LightComponent(glm::vec3&& position, glm::vec3&& ambient, glm::vec3&& diffuse, glm::vec3&& specular, float constant, float linear, float quadratic)
    :position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant), linear(linear), quadratic(quadratic) {};
};

struct CameraComponent: public Component {
    CameraComponent(glm::vec3 pos, unsigned index) :camIndex(index) { camera = Camera(pos); }
    CameraComponent(Camera camera, unsigned index) : camera(camera), camIndex(index) {};
    CameraComponent(float x, float y, float z, unsigned index) :camIndex(index) {camera = Camera(glm::vec3(x, y, z));};

    Camera camera;
    void setCameraPos(glm::vec3 pos){
        camera.Position = pos;
    }

    glm::mat4 getViewMatrix(){
        return camera.GetViewMatrix();
    }

    unsigned camIndex;

};



#endif //PROJECT_BASE_COMPONENT_H
