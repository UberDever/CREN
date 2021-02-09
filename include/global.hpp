//
// Created by uberdever on 26.05.2020.
//

#ifndef CREN_GLOBAL_HPP
#define CREN_GLOBAL_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <fstream>
#include <iostream>
#include <cmath>

//global - initially for interscene communication
namespace global
{
    namespace cfg
    {
        //global vars
        inline int* resListW() {static int var[6] = {640, 800, 1280, 1600, 1920, 2560}; return var;}
        inline int* resListH(){ static int var[6] = {480, 600, 720, 900, 1080, 1440}; return var; }

        inline int& curChosenRes(){ static int var {1}; return var; }
        inline int& scrRenW () {static int var {resListW()[curChosenRes()]}; return var;}
        inline int& scrRenH () { static int var {resListH()[curChosenRes()]}; return var;}
        inline int& scrUIW () {static int var {0}; return var;}
        inline int& scrUIH () {static int var {0}; return var;}

        //game vars
        inline int& pl_invW(){ static int var{10}; return var; }
        inline int& pl_invH(){ static int var{4}; return var; }

        inline float& pl_planeSize(){ static float var{1.f}; return var; }
        inline float& pl_rotSpeed(){ static float var{.3f}; return var; }
        inline float& mp_wallHeight(){ static float var{1.f}; return var; }
        inline bool& isFullscreen(){ static bool var{false}; return var; }
    }

    //graphics
    inline SDL_Window*& window() {static SDL_Window* var{nullptr}; return var;}
    inline SDL_Renderer*& renderer() {static SDL_Renderer* var{nullptr}; return var;}
    inline SDL_Texture*& gameScreen() {static SDL_Texture* var{nullptr}; return var;}
    inline uint32_t*& gameScreenBuffer() {static uint32_t* var{nullptr}; return var;}
}

//Module namespaces for intermodule communication

namespace gut
{
}

namespace UI
{
    void setupUI(); //Used for changing UI according to resolution Used in gut
}

namespace items
{
}

#endif //CREN_GLOBAL_HPP
