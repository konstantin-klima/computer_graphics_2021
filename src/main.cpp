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
#include "Entity/ModelManager.h"
#include "Entity/ShaderManager.h"
#include "Controller/PlayerController.h"
#include "Controller/RenderController.h"
#include "Controller/LevelController.h"
#include "Controller/PhysicsController.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

GLFWwindow *setupWindow();
void loadResources();
void loadModels();
void loadShaders();
void initControllers();


float lastX = Settings::SCR_WIDTH / 4.0f;
float lastY = Settings::SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    auto window = setupWindow();
    loadResources();
    initControllers();

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        PhysicsController::update();

        PlayerController::processInput(window);
        PlayerController::update();



        glm::vec3 clearColor = glm::vec3(0);
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderController::render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow *setupWindow() {
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    return window;
}

void initControllers() {
    PhysicsController::init();
    PlayerController::init(PhysicsController::getPhysicsCommon(), PhysicsController::getWorld());
    RenderController::init();
    LevelController::init(PhysicsController::getPhysicsCommon(), PhysicsController::getWorld());

}

void loadResources(){
    loadModels();
    loadShaders();
}

void loadModels() {
    auto arenaModel = new Model("resources/objects/arena/arena.obj");
    arenaModel->SetShaderTextureNamePrefix("material.");

    ModelManager::getManager().addModel("arena", arenaModel);
}

void loadShaders() {
    auto basic = new Shader("resources/shaders/basic.vs", "resources/shaders/basic.fs");
    ShaderManager::getManager().addShader("basic", basic);

    auto skybox = new Shader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
    ShaderManager::getManager().addShader("skybox", skybox);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

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
    PlayerController::processMouse(xoffset, yoffset);
}