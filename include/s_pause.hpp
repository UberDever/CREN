//
// Created by uberdever on 19.04.2020.
//

#ifndef CREN_S_PAUSE_HPP
#define CREN_S_PAUSE_HPP

#include "util.hpp"
#include "gut.hpp"

namespace pause
{
    using namespace util;
    using namespace gut;
    using namespace game;

    e_exitCodes init();
    e_gameStates event();
    e_gameStates update();
    void render(float);
    void clean();
}

#endif //CREN_S_PAUSE_HPP
