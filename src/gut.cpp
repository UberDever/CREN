//
// Created by uberdever on 20.02.2020.
//

#include "gut.hpp"

namespace gut
{

    SDL_Window* g_wind;
    SDL_Renderer* g_rend;

    SDL_Surface* temp_surf = nullptr; //for global use in gut functions

    /************************************************************/

    SDL_Window* getW() { return g_wind; }
    SDL_Renderer* getR() { return g_rend; }

    /************************************************************/

    void gut_raycast()
    {
        static Player& pl = getPlayer();

    }

    /************************************************************/

    e_exitCodes gut_init() {

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            return e_exitCodes::GUT_SDL_ERR;

        if (TTF_Init() == -1)
            return e_exitCodes::GUT_TTF_ERR;

        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
            return e_exitCodes::GUT_IMG_ERR;

        g_wind = SDL_CreateWindow("CREN", 0, 0, cfg::scrW(), cfg::scrH(), SDL_WINDOW_SHOWN);
        if (g_wind == nullptr)
            return e_exitCodes::GUT_SDL_ERR;

        g_rend = SDL_CreateRenderer(g_wind, -1, SDL_RENDERER_SOFTWARE);
        if (g_rend == nullptr)
            return e_exitCodes::GUT_SDL_ERR;

        return e_exitCodes::OK;
    }

    /************************************************************/

    void gut_clean() {
        SDL_DestroyRenderer(g_rend);
        SDL_DestroyWindow(g_wind);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
    }

}
