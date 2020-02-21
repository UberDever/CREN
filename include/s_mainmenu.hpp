//
// Created by uberdever on 19.02.2020.
//

#ifndef CREN_S_MAINMENU_HPP
#define CREN_S_MAINMENU_HPP

#include "util.hpp"

namespace mainmenu
{
    using namespace util;

    class DATA
    {
    public:
        ~DATA() {}
    };

    e_exitCodes init();
    e_gameStates event();
    e_gameStates update();
    void render();
    void clean();
}


#endif //CREN_S_MAINMENU_HPP
