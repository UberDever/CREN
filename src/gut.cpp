//
// Created by uberdever on 20.02.2020.
//

#include "gut.hpp"

namespace gut
{
    SDL_Window* g_wind;
    SDL_Renderer* g_rend;

    void (*pTransformFunc[5])(int, int) = {nullptr}; //pointers to transform functions

    SDL_Surface* temp_surf = nullptr; //for global use in gut functions

    /************************************************************/

    SDL_Window* getW() { return g_wind; }
    SDL_Renderer* getR() { return g_rend; }

    /************************************************************/

    void _scale(int _w, int _h)
    {
        SDL_Surface* _surf = SDL_CreateRGBSurface(0, _w, _h, 32, 0, 0, 0, 0);
        int w = temp_surf->w;
        int h = temp_surf->h;
        int x_ratio = (int)((w << 16) / _w);
        int y_ratio = (int)((h << 16) / _h);
        uint32_t* pOut = static_cast<uint32_t *>(_surf->pixels);
        uint32_t* pIn = static_cast<uint32_t *>(temp_surf->pixels);

        for (int i = 0; i < _h; i++)
        {
            for (int j = 0; j < _w; j++)
            {
                int px = (x_ratio * j) >> 16;
                int py = (y_ratio * i) >> 16;
                pOut[(i * _w) + j] = pIn[(py * w) + px];
            }
        }
        temp_surf = _surf;
    }

    SDL_Surface* gut_transformSurf(SDL_Surface* inSurf, int _w, int _h, uint8_t flag)
    {
        temp_surf = inSurf;
        (*pTransformFunc[flag])(_w, _h);
    }

    void gut_draw()
    {
        //static SDL_Texture* examTexture = SDL_CreateTextureFromSurface(g_rend, examSurf);
        int w = 32;
        int h = 32;
        static SDL_Rect out = {0, 0, w, h};
        gut_transformSurf(temp_surf, w, h, 0);
        SDL_RenderCopy(g_rend, SDL_CreateTextureFromSurface(g_rend, temp_surf), 0, &out);
        SDL_RenderPresent(g_rend);
    }

    /************************************************************/

    e_exitCodes gut_init() {

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            return e_exitCodes::GUT_SDL_ERR;

        if (TTF_Init() == -1)
            return e_exitCodes::GUT_TTF_ERR;

        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
            return e_exitCodes::GUT_IMG_ERR;

        g_wind = SDL_CreateWindow("CREN", 0, 0, 320, 240, SDL_WINDOW_SHOWN);
        if (g_wind == nullptr)
            return e_exitCodes::GUT_SDL_ERR;

        g_rend = SDL_CreateRenderer(g_wind, -1, SDL_RENDERER_SOFTWARE);
        if (g_rend == nullptr)
            return e_exitCodes::GUT_SDL_ERR;

        pTransformFunc[0] = _scale;

        temp_surf = res_loadPNG("../data/textures/wallHIRES.png");

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
