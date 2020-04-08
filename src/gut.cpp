//
// Created by uberdever on 20.02.2020.
//

#include "gut.hpp"

namespace gut
{
    game::GAME_DATA* pd; // for convenience

    SDL_Window* g_wind;
    SDL_Renderer* g_rend;

    SDL_Surface* scrSurf = nullptr;
    SDL_Surface* temp_surf = nullptr; //for global use in gut functions

    /************************************************************/

    SDL_Window* getW() { return g_wind; }
    SDL_Renderer* getR() { return g_rend; }

    /************************************************************/

    void drawVertical(int x, int yS, int yE)
    {
        auto* buf = static_cast<uint32_t *>(scrSurf->pixels);
        for (int i = yS; i < yE; ++i) {
            buf[i * getCFG().scrW + x] = 0xFF00FF00;
        }
    }

    void gut_raycast()
    {
        //pd->map->id[static_cast<int>((int)pd->pl->pos.y * pd->map->w + (int)pd->pl->pos.x)] = 'X';
        /*for (int i = 0; i < pd->map->h; ++i) {
            for (int j = 0; j < pd->map->w; ++j) {
                std::cout << (char)pd->map->id[i * pd->map->w + j];
            }
            std::cout << std::endl;
        }*/
        auto* buf = static_cast<uint32_t*>(scrSurf->pixels);
        for (int k = 0; k < getCFG().scrH; ++k) {
            for (int i = 0; i < getCFG().scrW; ++i) {
                buf[k * getCFG().scrW + i] = 0;
            }
        }

        auto* map = pd->mp->id;
        int scrW = getCFG().scrW;
        int scrH = getCFG().scrH;

        for (int x = 0; x < scrW; ++x) {
            float cam = 2.f * (float)x / (float)scrW - 1;
            v2<float> ray = pd->pl->dir + (pd->pl->plane * cam);
            v2<int> pos {static_cast<int>(pd->pl->pos.x), static_cast<int>(pd->pl->pos.y)};
            v2<float> side;
            v2<float> delta {std::fabs(1 / ray.x), std::fabs(1 / ray.y)};
            //v2<float> delta {(ray.y == 0) ? 0 : ((ray.x == 0) ? 1 : std::fabs(1 / ray.x)), (ray.x == 0) ? 0 : ((ray.y == 0) ? 1 : std::fabs(1 / ray.y))};
            float pwd = 0;
            v2<int> step;
            bool hit = false, NS = true; //North-South

            if (ray.x > 0)
            {
                step.x = 1;
                side.x = ((float)pos.x + 1 - pd->pl->pos.x) * delta.x;
            } else
            {
                step.x = -1;
                side.x = (pd->pl->pos.x - pos.x) * delta.x;
            }
            if (ray.y > 0)
            {
                step.y = 1;
                side.y = ((float)pos.y + 1 - pd->pl->pos.y) * delta.y;
            } else
            {
                step.y = -1;
                side.y = (pd->pl->pos.y - pos.y) * delta.y;
            }
            while (!hit)
            {
                if (side.x < side.y)
                {
                    side.x += delta.x;
                    pos.x += step.x;
                    NS = true;
                } else
                {
                    side.y += delta.y;
                    pos.y += step.y;
                    NS = false;
                }
                if (map[pos.y * pd->mp->w + pos.x] == '1') hit = true;
            }

            if (NS) pwd = (pos.x - pd->pl->pos.x + (1.f - step.x) / 2) / ray.x;
            else    pwd = (pos.y - pd->pl->pos.y + (1.f - step.y) / 2) / ray.y;

            int line = (int)(getCFG().mp_wallHeight * (float)scrH / pwd);
            int dS = -line / 2 + scrH / 2; dS = dS < 0 ? 0 : dS;
            int dE = line / 2 + scrH / 2; dE = dE >= scrH ? scrH - 1 : dE;

            drawVertical(x, dS, dE);
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(g_rend, scrSurf);
        SDL_RenderCopy(g_rend, tex, 0, 0);
        SDL_DestroyTexture(tex);
        SDL_RenderPresent(g_rend);
    }

    /************************************************************/

    e_exitCodes gut_init() {

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            return GUT_SDL_ERR;

        if (TTF_Init() == -1)
            return GUT_TTF_ERR;

        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
            return GUT_IMG_ERR;

        g_wind = SDL_CreateWindow("CREN", 0, 0, getCFG().scrW, getCFG().scrH, SDL_WINDOW_SHOWN | (getCFG().isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
        if (g_wind == nullptr)
            return GUT_SDL_ERR;

        g_rend = SDL_CreateRenderer(g_wind, -1, SDL_RENDERER_SOFTWARE);
        if (g_rend == nullptr)
            return GUT_SDL_ERR;

        pd = game::getData();

        scrSurf = SDL_CreateRGBSurface(0, getCFG().scrW, getCFG().scrH, 32, 0, 0, 0, 0);
        auto* buf = static_cast<uint32_t*>(scrSurf->pixels);
        for (int k = 0; k < getCFG().scrH; ++k) {
            for (int i = 0; i < getCFG().scrW; ++i) {
                buf[k * getCFG().scrW + i] = 0;
            }
        }

        return OK;
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
