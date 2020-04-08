//
// Created by uberdever on 18.03.2020.
//

#include "s_temp.hpp"

namespace temp
{
    e_exitCodes init() {
        return e_exitCodes::OK;
    }

    e_gameStates event() {
        return e_gameStates::TEMP;
    }

    e_gameStates update() {
        return e_gameStates::TEMP;
    }

    void render(float) {

    }

    void clean() {

    }
}