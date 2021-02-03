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
#include "Controller/LevelController.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

//void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void processInput(GLFWwindow *window);
GLFWwindow* setupWindow();
void loadModels();
void loadShaders();
void initControllers(reactphysics3d::PhysicsWorld *world);

// settings
// TODO Izmesti u PC
reactphysics3d::PhysicsCommon physicsCommon;

// camera

float lastX = Settings::SCR_WIDTH / 2.0f;
float lastY = Settings::SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    auto window = setupWindow();
    loadModels();
    loadShaders();

    reactphysics3d::PhysicsWorld *world = physicsCommon.createPhysicsWorld();
    initControllers(world);

    const reactphysics3d::decimal timeStep = 1.0f / 60.0f;
    float accumulator = 0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        accumulator += deltaTime;
        while (accumulator >= timeStep) {
            world->update(timeStep);
            accumulator -= timeStep;
        }

        PlayerController::processInput(window, deltaTime);
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

        RenderController::render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
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

GLFWwindow* setupWindow(){
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow *window = glfwCreateWindow(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    return window;
}

// TODO Izmesti Physics related stvari u Physics controler i komponente
void initControllers(reactphysics3d::PhysicsWorld *world) {
    PlayerController::init();
    RenderController::init();
    LevelController::init(world);

}

void loadModels() {
    auto arenaModel = new Model("resources/objects/arena/arena.obj", physicsCommon);
    arenaModel->SetShaderTextureNamePrefix("material.");

    ModelManager::getManager().addModel("arena", arenaModel);
}

void loadShaders(){
    auto basic = new Shader("resources/shaders/2.model_lighting.vs", "resources/shaders/2.model_lighting.fs");

    ShaderManager::getManager().addShader( "basic", basic);
}