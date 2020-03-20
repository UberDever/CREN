//
// Created by uberdever on 18.03.2020.
//

#ifndef CREN_S_TEMP_HPP
#define CREN_S_TEMP_HPP

#include "util.hpp"

namespace temp
{
    using namespace util;

    e_exitCodes init();
    e_gameStates event();
    e_gameStates update();
    void render();
    void clean();
}

#endif //CREN_S_TEMP_HPP
