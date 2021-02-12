//
// Created by konstantin on 03/02/2021.
//

#ifndef PROJECT_BASE_CONSTANTS_H
#define PROJECT_BASE_CONSTANTS_H

namespace Settings{
    const unsigned SCR_WIDTH = 1366;
    const unsigned SCR_HEIGHT = 768;

    enum CollisionCategory {
        ARENA = 0x0002,
        SPELL = 0X0004,
        PLAYER1 = 0x008,
        PLAYER2 = 0x010,
        PLAYER = 0x020
    };
}




#endif //PROJECT_BASE_CONSTANTS_H
