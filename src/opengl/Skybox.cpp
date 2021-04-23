//
// Created by konstantin on 04/02/2021.
//

#include "Skybox.h"

Skybox::Skybox()
{
	loadSkybox(defaultPath);
	scaleSkybox();
	prepareBuffers();
}

Skybox::Skybox(const std::string& skyboxDirectory)
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	scaleSkybox();
	loadSkybox(skyboxDirectory);
	prepareBuffers();
}

void Skybox::loadSkybox(const std::string& skyboxDirectory)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, channels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		auto fullPath = skyboxDirectory + "/" + faces[i] + ".jpg";
		unsigned char* tex =
			stbi_load(fullPath.c_str(), &width, &height, &channels, 0);

		if (tex)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
						 height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
		else
			std::cout << "Failed to load skybox texture file: " << faces[i]
					  << std::endl;

		stbi_image_free(tex);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	texture = textureID;
}

void Skybox::prepareBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_verts), &m_verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
}

void Skybox::Draw(Shader& shader)
{
	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void Skybox::scaleSkybox(float v_scale)
{
	unsigned n = 108;
	for (unsigned i = 0; i < n; ++i) {
		m_verts[i] = v_scale * verts[i];
	}
}
