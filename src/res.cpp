//
// Created by uberdever on 20.02.2020.
//

#include "res.hpp"

namespace res
{
    constexpr int fontNum = 4;

    TTF_Font* g_fontLib[fontNum] = {nullptr};

    SDL_Surface *res_loadPNG(const char *path) {
        SDL_Surface* tmp = nullptr;
        tmp = IMG_Load(path);
        return tmp;
    }

    TTF_Font* res_loadTTF(const char *path, int size) {
        TTF_Font* tmp = nullptr;
        tmp = TTF_OpenFont(path, size);
        return tmp;
    }

    TTF_Font *res_getFont(int size) {
        return g_fontLib[size];
    }

    e_exitCodes res_init()
    {
        //font part
        int fontSizes[fontNum] = {18, 24, 32, 48};
        constexpr char* fontPath = "../data/fonts/font.ttf";
        for (int i = 0; i < fontNum; i++)
        {
            g_fontLib[i] = res_loadTTF(fontPath, fontSizes[i]);
            if (g_fontLib[i] == nullptr)
                return e_exitCodes::RES_TTF_ERR;
        }

        //image part

        return e_exitCodes::OK;
    }

    void res_clean() {
        for (int i = 0; i < fontNum; i++)
        {
            TTF_CloseFont(g_fontLib[i]);
        }
    }
}


