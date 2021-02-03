//
// Created by konstantin on 02/02/2021.
//

#ifndef PROJECT_BASE_COMPONENT_H
#define PROJECT_BASE_COMPONENT_H


#include <glm/vec3.hpp>
#include <learnopengl/camera.h>
#include <learnopengl/shader.h>
#include <learnopengl/model.h>


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

struct CameraComponent: public Component {
    CameraComponent(glm::vec3 pos, unsigned index) :camIndex(index) { camera = Camera(pos); }
    CameraComponent(Camera cam, unsigned index) : camera(camera), camIndex(index) {};
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

struct ShaderComponent: public Component {
    std::unordered_map<std::string, Shader*> shaders;

    ShaderComponent() = default;

    void addShader(const std::string& name, Shader* s) {
        shaders[name] = s;
    }

    void removeShader(const std::string& name){
        auto s = shaders.find(name);
        if(s != shaders.end()){
            shaders.erase(s);
        }
    }

    Shader* getShader(const std::string& name) const {
        auto it = shaders.find(name);
        if(it != shaders.end())
            return it->second;
        else{
            return nullptr;
        }


    }

    std::vector<Shader*> getAllShaders() const {
        std::vector<Shader*> res;
        for(const auto& it : shaders){
            res.push_back(it.second);
        }

        return res;
    }
};

struct ModelComponent: public Component {
    explicit ModelComponent(Model* model) :model(model) {};

    void setModel(Model* model){
        this->model = model;
    }

    Model* getModel() const {
        return model;
    }

private:
    Model* model;
};


#endif //PROJECT_BASE_COMPONENT_H
