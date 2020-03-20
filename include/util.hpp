//
// Created by uberdever on 18.02.2020.
//

#ifndef CREN_UTIL_H
#define CREN_UTIL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <cstdint>
#include <iostream>
#include <cmath>

//cfg - all configuration parameters, useful for application.
namespace cfg
{
    constexpr uint32_t _mapW = 10;
    constexpr uint32_t _mapH = 10;

    constexpr uint32_t mapW() {return _mapW;};
    constexpr uint32_t mapH() {return _mapH;};

    uint32_t scrW();
    uint32_t scrH();
}

//util - all utility constructions, does not required to be in particular context, or used by system itself
//includes: enums, struct/class prototypes, general use functions
namespace util
{
    //game enums
    enum e_exitCodes : uint32_t
    {
        OK          =  0x00,
        GUT_SDL_ERR =  0x01,
        GUT_TTF_ERR =  0x02,
        GUT_IMG_ERR =  0x04,
        RES_TTF_ERR =  0x08,
        RES_GAME_ERR = 0x10,
        /*_ERR = 0x04,
        _ERR = 0x08,
        _ERR = 0x10,
        _ERR = 0x20,*/
    };

    enum e_gameStates : uint8_t
    {
        EXIT      = 0x00,
        MAIN_MENU = 0x01,
        GAMEPLAY  = 0x02,
        TEMP      = 0x03
    };

    //Vec2 struct and related functions
    template <typename T>
    struct v2
    {
        T x; T y;

        explicit v2(T _x, T _y) {x = _x; y = _y;}


        inline T norm1() {return fabs(x) + fabs(y);}
        inline T norm2() {return sqrt(x * x + y * y);}

        inline v2<T>& operator+=(const v2<T>&rhs) {x += rhs.x; y += rhs.y; return *this;}
        inline v2<T>& operator-=(const v2<T>& rhs) {x -= rhs.x; y -= rhs.y; return *this;}
    };

    template <typename T>
    inline v2<T> operator+ (const v2<T>& lhs, const v2<T>& rhs)
    {
        return v2<T>{lhs.x + rhs.x, lhs.y + rhs.y};
    }

    template <typename T>
    inline v2<T> operator- (const v2<T>& lhs, const v2<T>& rhs)
    {
        return v2<T>{lhs.x - rhs.x, lhs.y - rhs.y};
    }

    template <typename T>
    inline T operator* (const v2<T>&lhs, const v2<T>& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const v2<T>& vec)
    {
        os << "X: " << +vec.x << " Y: " << +vec.y;
        return os;
    }

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
    struct GameEvent
    {
        int32_t mx, my; // mouse x and y coordinates (x coordinate is key pressed (sdl_event is union))
        int32_t& key;
        uint8_t mbutton, type;

        GameEvent(): mx(0), my(0), key(mx), mbutton(0), type(8) {}
    };

    /************************************************************/

    constexpr int SDL_ParseCount = 8;

    GameEvent parse_event(SDL_Event* event);

    //generic use functions
    template <class T>
    inline T MAX(T a, T b)
    {
        return a > b ? a : b;
    }
    template <class T>
    inline T MIN(T a, T b)
    {
        return a < b ? a : b;
    }
}

//res - all outer world resource handling
namespace res
{
    using namespace util;

    e_exitCodes res_init();
    void res_clean();

    TTF_Font* res_getFont(int size);

    SDL_Surface* res_loadPNG(const char* path);
    TTF_Font* res_loadTTF(const char* path, int size);
}

//math - all math, used in project (it can be not much, but it still required)
namespace math
{
    void calc_trig (float beta, float* s, float* c);

    util::e_exitCodes math_init();
}

#endif //CREN_UTIL_H
