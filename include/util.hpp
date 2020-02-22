//
// Created by uberdever on 18.02.2020.
//

#ifndef CREN_UTIL_H
#define CREN_UTIL_H

#include <SDL.h>
#include <cstdint>
#include <iostream>

//util - all utility constructions, does not required to be in particular context, or used by system itself
//includes: enums, struct/class prototypes, general use functions
namespace util
{
    //game enums
    enum e_exitCodes : uint32_t
    {
        OK          = 0x00,
        GUT_SDL_ERR = 0x01,
        GUT_TTF_ERR = 0x02,
        GUT_IMG_ERR = 0x04,
        RES_TTF_ERR = 0x08,
        RES_IMG_ERR = 0x10
        /*_ERR = 0x04,
        _ERR = 0x08,
        _ERR = 0x10,
        _ERR = 0x20,*/
    };

    enum e_gameStates : uint8_t
    {
        EXIT      = 0x00,
        MAIN_MENU = 0x01,
        GAMEPLAY  = 0x02
    };

    //scene prototype
    struct Scene
    {
        Scene() { init = nullptr; event = update = nullptr; render = clean = nullptr;};

        e_exitCodes init_scene(e_exitCodes (*_init)(), e_gameStates (*_event)(), e_gameStates (*_update)(), void (*_render)(), void (*_clean)())
        {
            init = _init;
            event = _event;
            update = _update;
            render = _render;
            clean = _clean;
            return init();
        };
        ~Scene() { clean(); }

        e_exitCodes (*init)();
        e_gameStates (*event)();
        e_gameStates (*update)();
        void (*render)();
        void (*clean)();
    };

    //custom event struct
    struct c_gameEvent
    {
        int32_t mx, my; // mouse x and y coordinates (x coordinate is key pressed (sdl_event is union))
        int32_t& key;
        uint8_t mbutton, type;

        c_gameEvent(): mx(0), my(0), key(mx), mbutton(0), type(8) {}
    };

    constexpr int SDL_ParseCount = 8;

    c_gameEvent parse_event(SDL_Event* event);
}

#endif //CREN_UTIL_H
