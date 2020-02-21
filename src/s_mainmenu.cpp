//
// Created by uberdever on 19.02.2020.
//

#include "s_mainmenu.hpp"

namespace mainmenu
{
    DATA* pd;
    e_exitCodes init() {
        DATA* pd = new DATA();
        return static_cast<e_exitCodes>(e_exitCodes::OK || !pd);
    }

    e_gameStates event() {
        return e_gameStates::MAIN_MENU;
    }

    e_gameStates update() {
        return e_gameStates::MAIN_MENU;
    }

    void render() {

    }

    void clean() {
        delete pd;
    }
}
