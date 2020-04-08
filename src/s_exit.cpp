//
// Created by uberdever on 19.02.2020.
//

#include "s_exit.hpp"

namespace Void
{
    e_exitCodes init() {
        return e_exitCodes::OK;
    }

    e_gameStates event() {
        return e_gameStates::EXIT;
    }

    e_gameStates update() {
        return e_gameStates::EXIT;
    }

    void render(float) {

    }

    void clean() {

    }
}