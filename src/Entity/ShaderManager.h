//
// Created by konstantin on 03/02/2021.
//

#ifndef PROJECT_BASE_SHADERMANAGER_H
#define PROJECT_BASE_SHADERMANAGER_H

#include <vector>
#include <algorithm>
#include <memory>
#include "../include/learnopengl/shader.h"

struct ShaderManager {

	static ShaderManager& getManager()
	{
		static ShaderManager instance;
		return instance;
	}

	ShaderManager(ShaderManager const&) = delete;
	void operator=(ShaderManager const&) = delete;

	void addShader(const std::string& name, Shader* s)
	{
		m_shaders[name] = std::make_unique<Shader>(*s);
	}
	void addShader(const std::string& name, std::unique_ptr<Shader>&& s)
	{
		m_shaders[name] = (std::move(s));
	}

	void clearShaders() { m_shaders.clear(); }

	Shader* getShader(const std::string& name) const
	{
		for (const auto& it : m_shaders) {
			if (it.first == name)
				return it.second.get();
		}

		return nullptr;
	}

	std::vector<Shader*> getAllShaders() const
	{
		std::vector<Shader*> res;
		for (const auto& it : m_shaders) {
			res.push_back(it.second.get());
		}

		return res;
	}

  private:
	std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders {};
	ShaderManager() = default;
};
#endif // PROJECT_BASE_SHADERMANAGER_H
