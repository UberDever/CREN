//
// Created by uberdever on 20.02.2020.
//

#ifndef CREN_GUT_HPP
#define CREN_GUT_HPP

#include "util.hpp"
#include "game.hpp"

namespace gut
{
    using namespace util;
    using namespace res;
    using namespace game;

    e_exitCodes gut_init();
    void gut_clean();

    void gut_raycast();

    SDL_Window* getW();
    SDL_Renderer* getR();
}

#endif //CREN_GUT_HPP
