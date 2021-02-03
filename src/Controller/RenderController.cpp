//
// Created by konstantin on 03/02/2021.
//

#include "RenderController.h"

void RenderController::init() {
    loadShaders();
}

void RenderController::render() {
    updateLights();
    drawEntities();
}


void RenderController::loadShaders(){
    auto basic = new Shader("resources/shaders/2.model_lighting.vs", "resources/shaders/2.model_lighting.fs");

    ShaderManager::getManager().addShader( "basic", basic);
}

void RenderController::updateLights() {
    auto lights = EntityManager::getManager().getAllComponents<LightComponent>();
    auto shaders = ShaderManager::getManager().getAllShaders();

    for(auto shader : shaders){
        shader->use();
        auto pointLight = EntityManager::getManager().getAllComponents<LightComponent>()[0];
        for(const auto& light : lights){
            if(light->type == LIGHTS::SPECULAR){
                shader->setVec3("pointLight.position", pointLight->position);
                shader->setVec3("pointLight.ambient", pointLight->ambient);
                shader->setVec3("pointLight.diffuse", pointLight->diffuse);
                shader->setVec3("pointLight.specular", pointLight->specular);
                shader->setFloat("pointLight.constant", pointLight->constant);
                shader->setFloat("pointLight.linear", pointLight->linear);
                shader->setFloat("pointLight.quadratic", pointLight->quadratic);
                shader->setFloat("material.shininess", 32.0f);
            }
            else if(light->type == LIGHTS::SPOTLIGHT){

            }
        }
    }

}

void RenderController::drawEntities() {
    auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();
    auto entities = EntityManager::getManager().getEntitiesWithComponent<ModelComponent>();

    for(const auto player : players) {

        auto cameraComponent = player->getComponent<CameraComponent>();

        glViewport(cameraComponent->camIndex * Settings::SCR_WIDTH * 0.5, 0, Settings::SCR_WIDTH * 0.5,
                   Settings::SCR_HEIGHT);

        for (auto entity : entities) {

            if (entity == player)
                continue;
            auto shaderp = entity->getComponent<ShaderComponent>();
            auto model = entity->getComponent<ModelComponent>()->getModel();
            auto shader = shaderp->getShader("basic");

            if(shader){

                shader->setVec3("viewPosition", cameraComponent->camera.Position);
                glm::mat4 projection = glm::perspective(glm::radians(cameraComponent->camera.Zoom),
                                                        (float) Settings::SCR_WIDTH / (float) Settings::SCR_HEIGHT, 0.1f, 100.0f);
                glm::mat4 view = cameraComponent->getViewMatrix();
                shader->setMat4("projection", projection);
                shader->setMat4("view", view);

                // render the loaded model
                glm::mat4 modelMatrix = glm::mat4(1.0f);
                shader->setMat4("model", modelMatrix);
                model->Draw(*shader);
            }

        }
    }

}
