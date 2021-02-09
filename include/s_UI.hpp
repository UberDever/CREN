//
// Created by uberdever on 19.04.2020.
//

#ifndef CREN_S_UI_HPP
#define CREN_S_UI_HPP

#include "game.hpp"

namespace ui
{
    util::e_exitCodes init();
    util::e_gameStates event();
    util::e_gameStates update();
    void render(float);
    void clean();
}

#endif //CREN_S_UI_HPP
