#ifndef GLFW_HEAD_H
#define GLFW_HEAD_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>
#include "Entity/EntityManager.h"
#include "reactphysics3d/reactphysics3d.h"
#include "Controller/PlayerController.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

//void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1366;
const unsigned int SCR_HEIGHT = 768;

// camera

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    PlayerController::init();

    Shader ourShader("resources/shaders/2.model_lighting.vs", "resources/shaders/2.model_lighting.fs");

    // rp3d physicsCommon factory
    reactphysics3d::PhysicsCommon physicsCommon;

    // load models
    // -----------
    Model ourModel("resources/objects/arena/arena.obj", physicsCommon);
    ourModel.SetShaderTextureNamePrefix("material.");

    auto light = Entity();
    auto lc = LightComponent(
                glm::vec3(4.0f, 4.0, 0.0),
                glm::vec3(5, 5, 5),
                glm::vec3(0.9, 0.9, 0.9),
                glm::vec3(1.0, 1.0, 1.0),
                1.0f,
                0.09f,
                0.032f
            );
    light.addComponent<LightComponent>(lc);

    EntityManager::getManager().addEntity(&light);

    // ReactPhysics3D HelloWorld
    reactphysics3d::PhysicsWorld *world = physicsCommon.createPhysicsWorld();

    reactphysics3d::Vector3 position(0, 10, 0);
    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
    reactphysics3d::Transform transform(position, orientation);
    reactphysics3d::RigidBody *body = world->createRigidBody(transform);
    rp3d::Vector3 halfExtents(0.5, 0.75, 0.5);
    auto colliderShape = physicsCommon.createCapsuleShape(0.5, 1.6);
    body->addCollider(colliderShape, rp3d::Transform::identity());

    // arena
    auto arenaTransform = rp3d::Transform::identity();
    rp3d::RigidBody *arenaBody = world->createRigidBody(arenaTransform);
    arenaBody->setType(rp3d::BodyType::STATIC);
    arenaBody->addCollider(ourModel.concaveCollider->collider, arenaTransform);
    arenaBody->enableGravity(false);

    const reactphysics3d::decimal timeStep = 1.0f / 60.0f;
    float accumulator = 0;

    // draw in wireframe
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Physics
        accumulator += deltaTime;

        while (accumulator >= timeStep) {
            world->update(timeStep);
            accumulator -= timeStep;
        }

        /*
        auto camForce = rp3d::Vector3(
                programState->camera.Front.x,
                programState->camera.Front.y,
                programState->camera.Front.z
        );
        camForce *= deltaTime * 500.0f;
        body->applyForceToCenterOfMass(camForce);

        auto camPos = body->getTransform().getPosition();
        programState->camera.Position.x = camPos.x;
        programState->camera.Position.y = camPos.y;
        programState->camera.Position.z = camPos.z;
        */

        // input
        // -----
        PlayerController::processInput(window, deltaTime);
        // render
        // ------
        glm::vec3 clearColor = glm::vec3(0);
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniform
        // view/projection transformations
        ourShader.use();
        auto pointLight = EntityManager::getManager().getAllComponents<LightComponent>()[0];
        pointLight->position = glm::vec3(3.0f, 4.0f, 4.0f);
        ourShader.setVec3("pointLight.position", pointLight->position);
        ourShader.setVec3("pointLight.ambient", pointLight->ambient);
        ourShader.setVec3("poin tLight.diffuse", pointLight->diffuse);
        ourShader.setVec3("pointLight.specular", pointLight->specular);
        ourShader.setFloat("pointLight.constant", pointLight->constant);
        ourShader.setFloat("pointLight.linear", pointLight->linear);
        ourShader.setFloat("pointLight.quadratic", pointLight->quadratic);
        ourShader.setFloat("material.shininess", 32.0f);

        for(const auto player : players){

            auto cameraComponent = player->getComponent<CameraComponent>();
            glViewport(cameraComponent->camIndex * SCR_WIDTH * 0.5, 0, SCR_WIDTH*0.5, SCR_HEIGHT);

            ourShader.setVec3("viewPosition", cameraComponent->camera.Position);
            glm::mat4 projection = glm::perspective(glm::radians(cameraComponent->camera.Zoom),
                                                    (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = cameraComponent->getViewMatrix();
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);

            // render the loaded model
            glm::mat4 model = glm::mat4(1.0f);
            ourShader.setMat4("model", model);
            ourModel.Draw(ourShader);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
/*
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (programState->CameraMouseMovementUpdateEnabled)
        programState->camera.ProcessMouseMovement(xoffset, yoffset);
}
 */