//
// Created by uberdever on 20.02.2020.
//

#ifndef CREN_GUT_HPP
#define CREN_GUT_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "util.hpp"
#include "res.hpp"

namespace gut
{
    using namespace util;
    using namespace res;

    e_exitCodes gut_init();
    void gut_clean();

    void gut_draw();

    SDL_Window* getW();
    SDL_Renderer* getR();
}

#endif //CREN_GUT_HPP
