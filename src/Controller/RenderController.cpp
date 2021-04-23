//
// Created by konstantin on 03/02/2021.
//

#include "RenderController.h"
#include <assert.h>

void RenderController::init() { loadShaders(); }

void RenderController::render()
{
	clearGlBuffers();
	updateLights();
	drawEntities();
}

void RenderController::loadShaders() {}

void RenderController::updateLights()
{
	// Update spell light positions:
	auto spells = EntityManager::getManager()
					  .getEntitiesWithComponent<SpellPropertyComponent>();
	for (auto spell : spells) {
		auto pos = spell->getComponent<RigidBodyComponent>()->getPosition();
		auto lc = spell->getComponent<LightComponent>();

		lc->position = glm::vec3(pos.x, pos.y, pos.z);
	}

	auto lights =
		EntityManager::getManager().getAllComponents<LightComponent>();
	auto shaders = ShaderManager::getManager().getAllShaders();

	for (auto shader : shaders) {
		shader->use();
		shader->setFloat("material.shininess", 32.0f);

		unsigned pointLightNo = 0;
		unsigned spotlightNo = 0;

		for (const auto& light : lights) {
			if (light->type == LIGHTS::POINT) {

				std::ostringstream stream;
				stream << "pointLights[" << pointLightNo << "].";
				std::string prefix = stream.str();

				shader->setVec3(prefix + "position", light->position);
				shader->setVec3(prefix + "ambient", light->ambient);
				shader->setVec3(prefix + "diffuse", light->diffuse);
				shader->setVec3(prefix + "specular", light->specular);
				shader->setFloat(prefix + "constant", light->constant);
				shader->setFloat(prefix + "linear", light->linear);
				shader->setFloat(prefix + "quadratic", light->quadratic);

				pointLightNo++;
			} else if (light->type == LIGHTS::SPOTLIGHT) {
				std::ostringstream stream;
				stream << "spotLights[" << spotlightNo << "].";
				std::string prefix = stream.str();

				shader->setVec3(prefix + "position", light->position);
				shader->setVec3(prefix + "direction", light->direction);
				shader->setVec3(prefix + "ambient", light->ambient);
				shader->setVec3(prefix + "diffuse", light->diffuse);
				shader->setVec3(prefix + "specular", light->specular);
				shader->setFloat(prefix + "cutOff", light->cutOff);
				shader->setFloat(prefix + "quadratic", light->cutOffOuter);

				spotlightNo++;
			} else if (light->type == LIGHTS::DIRECT) {
				shader->setVec3("directLight.direction", light->direction);
				shader->setVec3("directLight.ambient", light->ambient);
				shader->setVec3("directLight.diffuse", light->diffuse);
				shader->setVec3("directLight.specular", light->specular);
			}
		}

		shader->setInt("pointLightNo", pointLightNo);
		shader->setInt("spotLightNo", spotlightNo);
	}
}

void RenderController::updateShaderView(Camera c)
{
	auto shaders = ShaderManager::getManager().getAllShaders();

	for (auto shader : shaders) {
		shader->use();
		shader->setVec3("viewPosition", c.Position);
	}
}

void RenderController::clearGlBuffers()
{
	glm::vec3 clearColor = glm::vec3(0);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderController::drawOtherPlayer(Entity* player)
{
	auto players =
		EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();

	Entity* other = nullptr;
	if (player == players[0])
		other = players[1];
	else
		other = players[0];

	auto playerShader = other->getComponent<ShaderComponent>()->getShader();
	auto playerTransform =
		other->getComponent<CollisionBodyComponent>()->getGLMTransform();
	playerTransform = glm::translate(playerTransform, glm::vec3(0, 0.5, 0));
	playerShader->use();
	glm::mat4 projection = glm::perspective(
		glm::radians(player->getComponent<CameraComponent>()->camera.Zoom),
		(float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = player->getComponent<CameraComponent>()->getViewMatrix();

	playerShader->setMat4("model", playerTransform);
	playerShader->setMat4("projection", projection);
	playerShader->setMat4("view", view);

	auto playerModel = other->getComponent<PlayerModelComponent>();

	playerModel->draw(playerShader);
}

void RenderController::drawEntities()
{
	auto players =
		EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();
	auto entities =
		EntityManager::getManager().getEntitiesWithComponent<ModelComponent>();
	for (const auto player : players) {

		auto cameraComponent = player->getComponent<CameraComponent>();
		updateShaderView(cameraComponent->camera);

		glViewport(cameraComponent->camIndex * Settings::SCR_WIDTH * 0.5, 0,
				   Settings::SCR_WIDTH * 0.5, Settings::SCR_HEIGHT);

		drawOtherPlayer(player);

		for (auto entity : entities) {

			if (entity == players[0] || entity == players[1]) {
				continue;
			}

			auto shaderComponent = entity->getComponent<ShaderComponent>();
			auto model = entity->getComponent<ModelComponent>()->getModel();
			Component* body = nullptr;
			if (entity->hasComponent<CollisionBodyComponent>())
				body = entity->getComponent<CollisionBodyComponent>();
			else if (entity->hasComponent<RigidBodyComponent>())
				body = entity->getComponent<RigidBodyComponent>();

			auto shader = shaderComponent->getShader();

			if (shader) {
				shader->use();
				shader->setVec3("viewPosition",
								cameraComponent->camera.Position);
				glm::mat4 projection = glm::perspective(
					glm::radians(cameraComponent->camera.Zoom),
					(float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT,
					0.1f, 100.0f);
				glm::mat4 view = cameraComponent->getViewMatrix();
				shader->setMat4("projection", projection);
				shader->setMat4("view", view);

				// render the loaded model
				glm::mat4 modelMatrix;
				if (body) {
					modelMatrix =
						((RigidBodyComponent*)body)->getGLMTransform();
				} else
					modelMatrix = glm::mat4(1.0f);

				shader->setMat4("model", modelMatrix);
				model->Draw(*shader);
			}
		}

		auto skyboxes = EntityManager::getManager()
							.getEntitiesWithComponent<SkyboxComponent>();
		if (skyboxes.size()) {
			assert(skyboxes.size() <= 1);
			auto skybox = skyboxes[0];
			auto shaderComponent = skybox->getComponent<ShaderComponent>();
			auto shader = shaderComponent->getShader();

			shader->use();
			shader->setVec3("viewPosition", cameraComponent->camera.Position);
			glm::mat4 projection = glm::perspective(
				glm::radians(cameraComponent->camera.Zoom),
				(float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT, 0.1f,
				100.0f);

			auto view = glm::mat4(glm::mat3(
				cameraComponent->getViewMatrix())); // remove translation from
													// the view matrix
			shader->setMat4("viewMatrix", view);
			shader->setMat4("projectionMatrix", projection);
			shader->setVec3("ambientLight", 0.4f, 0.1f, 0.1f);

			auto skyboxModel =
				skybox->getComponent<SkyboxComponent>()->getSkybox();
			skyboxModel.Draw(*shader);
		}
	}
}
