//
// Created by rentorious on 11/02/2021.
//

#ifndef PROJECT_BASE_PLAYERMODEL_H
#define PROJECT_BASE_PLAYERMODEL_H

#include <glad/glad.h>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include "learnopengl/shader.h"



class PlayerModel {
public:
    PlayerModel();

    void draw(Shader *shader);

    void setAlive(bool alive);


private:
    float scale = 1;

    unsigned int VAO, VBO, EBO;

    float cube[48] = {
            // POSITIONS-------NORMALS-------------TEXTURE 1
            -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
    };

    unsigned int indices[36] = {
            0, 1, 2, 2, 3, 0,
            3, 2, 6, 6, 7, 3,
            7, 6, 5, 5, 4, 7,
            4, 0, 3, 3, 7, 4,
            0, 1, 5, 5, 4, 0,
            1, 5, 6, 6, 2, 1
    };

    glm::vec4 aliveColor = glm::vec4(0.2, 0.88, 0.31, 1.0);
    glm::vec4 deadColor = glm::vec4(0.72, 0.15, 0.1, 1.0);
    glm::vec4 activeColor = glm::vec4(aliveColor);
};

#endif //PROJECT_BASE_PLAYERMODEL_H
