//
// Created by uberdever on 20.02.2020.
//

#include "gut.hpp"

namespace gut
{
    SDL_Texture* skybox = nullptr; //TODO REMOVE AFTER

    /************************************************************/

    void drawSky()
    {

    }

    /************************************************************/

    e_exitCodes gut_init() {

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            return GUT_SDL_ERR;

        if (TTF_Init() == -1)
            return GUT_TTF_ERR;

        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
            return GUT_IMG_ERR;

        global::window() = SDL_CreateWindow("CREN", 0, 0, global::cfg::scrRenW(), global::cfg::scrRenH(), SDL_WINDOW_SHOWN | (global::cfg::isFullscreen() ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) | SDL_WINDOW_OPENGL);
        SDL_GetWindowSize(global::window(), &global::cfg::scrUIW(), &global::cfg::scrUIH());
        if (global::window() == nullptr)
            return GUT_SDL_ERR;

        global::renderer() = SDL_CreateRenderer(global::window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        if (global::renderer() == nullptr)
            return GUT_SDL_ERR;

        SDL_SetRenderDrawBlendMode(global::renderer(), SDL_BLENDMODE_BLEND);

        global::gameScreen() = SDL_CreateTexture(global::renderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, global::cfg::scrRenW(), global::cfg::scrRenH());
        global::gameScreenBuffer() = new uint32_t[global::cfg::scrRenW() * global::cfg::scrRenH()];
        SDL_SetTextureBlendMode(global::gameScreen(), SDL_BLENDMODE_BLEND);

        textScaleFactor() = math::v2<float>{ (float)global::cfg::scrUIW() / 1920, (float)global::cfg::scrUIH() / 1080};

        SDL_Surface* tmp = res::res_loadPNG("../data/textures/skybox.png");
        //skybox = SDL_CreateTextureFromSurface(global::renderer(), tmp);
        SDL_FreeSurface(tmp);

        return OK;
    }

    /************************************************************/

    void gut_clean() {
        delete global::gameScreenBuffer();
        SDL_DestroyTexture(global::gameScreen());
        SDL_DestroyRenderer(global::renderer());
        SDL_DestroyWindow(global::window());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
    }

    void gut_updWin(int w, int h, uint32_t flags) {
        global::cfg::scrRenW() = w;
        global::cfg::scrRenH() = h;

        SDL_DestroyTexture(global::gameScreen());
        delete global::gameScreenBuffer();
        global::gameScreen() = SDL_CreateTexture(global::renderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, w, h);
        SDL_SetTextureBlendMode(global::gameScreen(), SDL_BLENDMODE_BLEND);
        global::gameScreenBuffer() = new uint32_t[w * h];

        SDL_SetWindowFullscreen(global::window(), flags);
        SDL_SetWindowSize(global::window(), w, h);

        SDL_GetWindowSize(global::window(), &global::cfg::scrUIW(), &global::cfg::scrUIH());
        textScaleFactor() = math::v2<float>{ (float)global::cfg::scrUIW() / 1920, (float)global::cfg::scrUIH() / 1080};
        UI::setupUI();
    }

}
