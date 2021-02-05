#ifndef GLFW_HEAD_H
#define GLFW_HEAD_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#include <glm/glm.hpp>
#include <learnopengl/filesystem.h>
#include <learnopengl/model.h>

#include "constants.h"

#include <iostream>
#include "Entity/EntityManager.h"
#include "Entity/ModelManager.h"
#include "Entity/ShaderManager.h"
#include "reactphysics3d/reactphysics3d.h"
#include "Controller/PlayerController.h"
#include "Controller/RenderController.h"
#include "Controller/PhysicsController.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

//void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void processInput(GLFWwindow *window);

// settings


// camera

float lastX = Settings::SCR_WIDTH / 2.0f;
float lastY = Settings::SCR_HEIGHT / 2.0f;
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
    GLFWwindow *window = glfwCreateWindow(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    PhysicsController::init();
    PlayerController::init(PhysicsController::getPhysicsCommon(), PhysicsController::getWorld());
    RenderController::init();


    // load models
    // -----------

    auto arenaModel = new Model("resources/objects/arena/arena.obj", PhysicsController::getPhysicsCommon());
    arenaModel->SetShaderTextureNamePrefix("material.");

    ModelManager::getManager().addModel("arena", arenaModel);

    auto arena = new Entity();
    arena->addComponent<ModelComponent>(arenaModel);
    auto arenaShader = ShaderComponent();
    arenaShader.addShader("basic", ShaderManager::getManager().getShader("basic"));
    arena->addComponent<ShaderComponent>(arenaShader);
    EntityManager::getManager().addEntity(arena);

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
    auto world = PhysicsController::getWorld();

    // arena
    auto arenaTransform = rp3d::Transform::identity();
    rp3d::RigidBody *arenaBody = world->createRigidBody(arenaTransform);
    arenaBody->setType(rp3d::BodyType::STATIC);
    arenaBody->addCollider(arenaModel->concaveCollider->collider, arenaTransform);
    arenaBody->enableGravity(false);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        PlayerController::update();
        PhysicsController::update();

        // -----
        PlayerController::processInput(window);
        // render
        // ------
        glm::vec3 clearColor = glm::vec3(0);
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderController::render();
        // don't forget to enable shader before setting uniform
        // view/projection transformations




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