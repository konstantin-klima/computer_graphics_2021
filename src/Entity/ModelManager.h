//
// Created by konstantin on 03/02/2021.
//

#ifndef PROJECT_BASE_MODELMANAGER_H
#define PROJECT_BASE_MODELMANAGER_H

#include <vector>
#include <algorithm>
#include <memory>
#include "../include/learnopengl/model.h"

struct ModelManager {

	static ModelManager& getManager()
	{
		static ModelManager instance;
		return instance;
	}

	ModelManager(ModelManager const&) = delete;
	void operator=(ModelManager const&) = delete;

	void addModel(const std::string& name, Model* s)
	{
		m_models[name] = std::make_unique<Model>(*s);
	}
	void addModel(const std::string& name, std::unique_ptr<Model>&& s)
	{
		m_models[name] = (std::move(s));
	}

	void clearModels() { m_models.clear(); }

	Model* getModel(const std::string& name) const
	{
		for (const auto& it : m_models) {
			if (it.first == name)
				return it.second.get();
		}

		return nullptr;
	}

	std::vector<Model*> getAllModels() const
	{
		std::vector<Model*> res;
		for (const auto& it : m_models) {
			res.push_back(it.second.get());
		}

		return res;
	}

  private:
	std::unordered_map<std::string, std::unique_ptr<Model>> m_models {};
	ModelManager() = default;
};
#endif // PROJECT_BASE_MODELMANAGER_H
