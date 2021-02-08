//
// Created by konstantin on 02/02/2021.
//

#ifndef PROJECT_BASE_COMPONENT_H
#define PROJECT_BASE_COMPONENT_H


#include <glm/vec3.hpp>
#include <learnopengl/camera.h>
#include <learnopengl/shader.h>
#include <learnopengl/model.h>

#include "reactphysics3d/reactphysics3d.h"

#include <iostream>

#include "ConcaveCollider.h"

// Base component code adapted from Nikola Sobajic
typedef unsigned ComponentTypeID;

struct Component {
    ComponentTypeID m_TypeId{0};
    bool m_Active{false};

    virtual ~Component() = default;

private:
    inline static ComponentTypeID m_MaxComponentTypeId = 0;

public:
    template<typename T>
    static ComponentTypeID GetComponentTypeID() {
        static_assert(std::is_base_of<Component, T>::value, "");
        static ComponentTypeID typeID = Component::m_MaxComponentTypeId++;
        return typeID;
    }
};

enum LIGHTS {
    SPECULAR,
    SPOTLIGHT
};

struct LightComponent : public Component {
    LIGHTS type{SPECULAR};
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 direction;
    float cutOff = 0;
    float cutOffOuter = 0;

    LightComponent(glm::vec3 &&position, glm::vec3 &&ambient, glm::vec3 &&diffuse, glm::vec3 &&specular, float constant,
                   float linear, float quadratic)
            : position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant),
              linear(linear), quadratic(quadratic) {};
};

struct CameraComponent : public Component {
    CameraComponent(glm::vec3 pos, unsigned index) : camIndex(index) { camera = Camera(pos); }

    CameraComponent(Camera camera, unsigned index) : camera(camera), camIndex(index) {};

    CameraComponent(float x, float y, float z, unsigned index) : camIndex(index) {
        camera = Camera(glm::vec3(x, y, z));
    };

    Camera camera;


    void setCameraPos(glm::vec3 pos) {
        camera.Position = pos;
    }

    void updateCameraVectors(float xoffset, float yoffset){
        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    glm::mat4 getViewMatrix() {
        return camera.GetViewMatrix();
    }

    unsigned camIndex;

};

struct ShaderComponent : public Component {
    std::unordered_map<std::string, Shader *> shaders;

    ShaderComponent() = default;

    void addShader(const std::string &name, Shader *s) {
        shaders[name] = s;
    }

    void removeShader(const std::string &name) {
        auto s = shaders.find(name);
        if (s != shaders.end()) {
            shaders.erase(s);
        }
    }

    Shader *getShader(const std::string &name) const {
        auto it = shaders.find(name);
        if (it != shaders.end())
            return it->second;
        else {
            return nullptr;
        }


    }

    std::vector<Shader *> getAllShaders() const {
        std::vector<Shader *> res;
        for (const auto &it : shaders) {
            res.push_back(it.second);
        }

        return res;
    }
};

struct ModelComponent : public Component {
    explicit ModelComponent(Model *model) : model(model) {};

    void setModel(Model *model) {
        this->model = model;
    }

    Model *getModel() const {
        return model;
    }

    std::vector<Mesh> getMeshes() const {
        return model->meshes;
    }

private:
    Model *model;
};

struct RigidBodyComponent : public Component {
    RigidBodyComponent(rp3d::RigidBody *body) : body(body) {};

    RigidBodyComponent(float x, float y, float z, float pitch, float yaw, rp3d::PhysicsWorld *world, bool env = false) {
        auto pos = rp3d::Vector3(x, y, z);
        auto orientation = rp3d::Quaternion::fromEulerAngles(yaw, pitch, 0);
        auto transform = rp3d::Transform(pos, orientation);
        prevTransform = transform;
        body = world->createRigidBody(transform);

        if (env) {
            body->setType(rp3d::BodyType::STATIC);
            body->enableGravity(false);
        }
    }

    void setMass(float mass) {
        body->setMass(mass);
    }

    float getMass() const {
        return body->getMass();
    }

    void setRigidBody(rp3d::RigidBody *body) {
        this->body = body;
    }

    rp3d::RigidBody *getRigidBody() const {
        return body;
    }

    rp3d::Vector3 getPosition() const {
        return body->getTransform().getPosition();
    }

    void setPosition(const rp3d::Vector3 &newPos) {
        auto orientation = body->getTransform().getOrientation();
        auto newTransform = rp3d::Transform(newPos, orientation);
        body->setTransform(rp3d::Transform(newTransform));
    }

    void setOrientation(float pitch, float yaw, float roll) {
        auto newOrientation = rp3d::Quaternion().fromEulerAngles(pitch, yaw, roll);
        auto oldPosition = body->getTransform().getPosition();
        body->setTransform(rp3d::Transform(oldPosition, newOrientation));
    }

    void setTransform(const rp3d::Transform &transform) {
        body->setTransform(transform);
    }

    rp3d::Transform getTransform() const {
        return body->getTransform();
    }


    glm::mat4 getGLMTransform() const {
        float transform[16];
        body->getTransform().getOpenGLMatrix(transform);

        return glm::mat4({
                                 {transform[0],  transform[1],  transform[2],  transform[3]},
                                 {transform[4],  transform[5],  transform[6],  transform[7]},
                                 {transform[8],  transform[9],  transform[10], transform[11]},
                                 {transform[12], transform[13], transform[14], transform[15]}
                         });
    }

    void setLinearVelocity(const rp3d::Vector3 vel) {
        body->setLinearVelocity(vel);
    }

    void setLinearVelocity(float x, float y, float z, float speed) {
        body->setLinearVelocity(rp3d::Vector3(x * speed, y * speed, z * speed));
    }

    void setLinearVelocity(const rp3d::Vector3 &dir, const float speed) {
        body->setLinearVelocity(dir * speed);
    }

    rp3d::Vector3 getLinearVelocity() const {
        body->getLinearVelocity();
    }

    void applyForceAtCenter(rp3d::Vector3 force) {
        body->applyForceToCenterOfMass(force);
    }

    glm::vec3 getGLMPosition() const {
        auto pos = body->getTransform().getPosition();

        return glm::vec3(pos.x, pos.y, pos.z);
    }

    rp3d::Collider *addCollider(rp3d::CollisionShape *shape) {
        return body->addCollider(shape, rp3d::Transform::identity());
    }

private:
    rp3d::RigidBody *body;
    rp3d::Transform prevTransform;
};

struct MovementComponent : public Component {
    MovementComponent(float dirx, float diry, float dirz, float speed)
            : direction(rp3d::Vector3(dirx, diry, dirz)), speed(speed) {};

    explicit MovementComponent(rp3d::Vector3 *dir, float speed) : direction(rp3d::Vector3(dir->x, dir->y, dir->z)),
                                                                  speed(speed) {};

    explicit MovementComponent(glm::vec3 *dir, float speed)
            : direction(rp3d::Vector3(dir->x, dir->y, dir->z)), speed(speed) {};

    rp3d::Vector3 getDirection() const {
        return direction;
    }

    glm::vec3 getGLMVDirection() const {
        return glm::vec3(direction.x, direction.y, direction.z);
    }

    void setDirection(rp3d::Vector3 *dir) {
        direction.x = dir->x;
        direction.y = dir->y;
        direction.z = dir->z;
    }

    float getSpeed() const {
        return speed;
    }

    void setVelocity(rp3d::Vector3 *vel) {
        speed = vel->length();
        direction.x = vel->x / speed;
        direction.y = vel->y / speed;
        direction.z = vel->z / speed;
    }


private:
    rp3d::Vector3 direction;
    float speed;
};


enum SPELL_TYPES {
    BULLET,
    PARABOLA
};

struct SpellPropertyComponent : public Component {
    SpellPropertyComponent(float damage, float range, SPELL_TYPES type)
            : damage(damage), range(range), type(type) {};

    float getDamage() const {
        return damage;
    }

    float getRange() const {
        return range;
    }

    SPELL_TYPES getType() const {
        return type;
    }

    void setDamage(const float damage) {
        this->damage = damage;
    }

    void setRange(const float range) {
        this->range = range;
    }

    void setType(const SPELL_TYPES type) {
        this->type = type;
    }

private:
    float damage;
    float range;
    SPELL_TYPES type;
};

enum COLLIDER_TYPES {
    BOX,
    SPHERE,
    CAPSULE,
    CONCAVE,
    CONVEX,
};

struct CapsuleColliderComponent : public Component {
    CapsuleColliderComponent(float radius, float height, rp3d::PhysicsCommon *physicsCommon) {
        shape = physicsCommon->createCapsuleShape(radius, height);
    }

    rp3d::CapsuleShape *getShape() const {
        return shape;
    }

private:
    rp3d::CapsuleShape *shape;
};

struct BoxColliderComponent : public Component {
    BoxColliderComponent(float halfX, float halfY, float halfZ, rp3d::PhysicsCommon *physicsCommon) {
        shape = physicsCommon->createBoxShape(rp3d::Vector3(halfX, halfY, halfZ));
    }

    rp3d::BoxShape *getShape() const {
        return shape;
    }

private:
    rp3d::BoxShape *shape;
};

struct ConcaveColliderComponent : public Component {
    ConcaveColliderComponent(std::vector<Mesh> meshes, rp3d::PhysicsCommon *physicsCommon) {
        collider = new ConcaveCollider(meshes, *physicsCommon);
    }

    rp3d::ConcaveMeshShape *getShape() const {
        return collider->getShape();
    }
private:
    ConcaveCollider *collider;
};

#endif //PROJECT_BASE_COMPONENT_H
