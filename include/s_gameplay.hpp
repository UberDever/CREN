//
// Created by uberdever on 18.02.2020.
//

#ifndef CREN_S_GAMEPLAY_HPP
#define CREN_S_GAMEPLAY_HPP

#include "game.hpp"

namespace gameplay
{
    using namespace util;
    using namespace game;
    using namespace gut;

    e_exitCodes init();
    e_gameStates event();
    e_gameStates update();
    void render(float);
    void clean();
}

#endif //CREN_S_GAMEPLAY_HPP
