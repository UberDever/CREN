//
// Created by uberdever on 20.02.2020.
//

#ifndef CREN_RES_HPP
#define CREN_RES_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "util.hpp"

namespace res
{
    using namespace util;

    e_exitCodes res_init();
    void res_clean();

    SDL_Surface* load_PNG(const char* path);
    TTF_Font* load_TTF(const char* path, int size);
}

#endif //CREN_RES_HPP
