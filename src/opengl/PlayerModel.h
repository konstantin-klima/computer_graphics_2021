//
// Created by rentorious on 11/02/2021.
//

#ifndef PROJECT_BASE_PLAYERMODEL_H
#define PROJECT_BASE_PLAYERMODEL_H

#include <glad/glad.h>
#include <string>
#include <sstream>

class PlayerModel {
public:
    PlayerModel();

    void draw();

    std::string toString() {
        std::stringstream ss;
        ss << "VBO " << VBO << ", EBO: " << EBO << ", VAO: " << VAO << std::endl;

        return ss.str();
    }

private:
    float scale = 1;

    unsigned int VAO, VBO, EBO;

    float cube[48] = {
            // POSITIONS-------NORMALS------------
            -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
    };

    float indices[36] = {
            0, 1, 2, 2, 3, 0,
            3, 2, 6, 6, 7, 3,
            7, 6, 5, 5, 4, 7,
            4, 0, 3, 3, 7, 4,
            0, 1, 5, 5, 4, 0,
            1, 5, 6, 6, 2, 1
    };
};

#endif //PROJECT_BASE_PLAYERMODEL_H
