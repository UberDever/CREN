//
// Created by uberdever on 20.02.2020.
//

#ifndef CREN_GUT_HPP
#define CREN_GUT_HPP

#include "util.hpp"

//gut - contains all graphic related utilities
namespace gut
{
    using namespace util;

    inline void setPixel(int x, int y)
    {
        global::gameScreenBuffer()[y + x] = 0xFFFFFF00;
    }

    inline void cls()
    {
        SDL_RenderClear(global::renderer());
        memset(global::gameScreenBuffer(), SDL_ALPHA_TRANSPARENT, global::cfg::scrRenW() * global::cfg::scrRenH() * sizeof(uint32_t));
    }

    inline math::v2<float>& textScaleFactor(){
        static math::v2<float> var;
        return var;
    }

    /************************************************************/

    void gut_updWin(int w, int h, uint32_t flags);

    e_exitCodes gut_init();
    void gut_clean();
}

#endif //CREN_GUT_HPP
