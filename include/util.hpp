//
// Created by uberdever on 18.02.2020.
//

#ifndef CREN_UTIL_H
#define CREN_UTIL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <fstream>
#include <iostream>
#include <cmath>

//cfg - all configuration parameters, useful for application.
namespace cfg
{
    struct CFG
    {
        //global vars
        int scrRenW;
        int scrRenH;
        int scrUIW;
        int scrUIH;

        //game vars
        float pl_planeSize;
        float pl_rotSpeed;
        float mp_wallHeight;

        bool isFullscreen;

        CFG():
                scrRenW{640},
                scrRenH{480},
                scrUIW{0},
                scrUIH{0},
                isFullscreen{true},

                pl_planeSize{.66666666f},
                pl_rotSpeed{.3f},
                mp_wallHeight{1.f}
        {}
    };

    CFG& getCFG();
}

//util - all utility constructions, does not required to be in particular context, or used by system itself
//includes: enums, struct/class prototypes, general use functions
namespace util
{
    //constexpr vars
    constexpr float PI = 3.14159265;
    constexpr int SDL_ParseCount = 8;

    //game enums
    enum e_exitCodes : uint32_t
    {
        OK            =  0x00,
        GUT_SDL_ERR   =  0x01,
        GUT_TTF_ERR   =  0x02,
        GUT_IMG_ERR   =  0x04,
        RES_TTF_ERR   =  0x08,
        RES_IMG_ERR   =  0x10,
        GAME_MAP_ERR  =  0x20,
        UI_CONF_ERR   =  0x40,
        UI_PARSE_ERR  =  0x80,
        UNUSED2_ERR   =  0x100
    };

    enum e_gameStates : uint8_t
    {
        EXIT      = 0x00,
        MAIN_MENU = 0x01,
        GAMEPLAY  = 0x02,
        PAUSE     = 0x03,
        TEMP      = 0x04
    };

    /************************************************************/

    //scene prototype
    struct SCENE
    {
        SCENE() { init = nullptr; event = update = nullptr; render = nullptr; clean = nullptr;};

        e_exitCodes init_scene(e_exitCodes (*_init)(), e_gameStates (*_event)(), e_gameStates (*_update)(), void (*_render)(float), void (*_clean)())
        {
            init = _init;
            event = _event;
            update = _update;
            render = _render;
            clean = _clean;
            return init();
        };
        ~SCENE() { clean(); }

        e_exitCodes (*init)();
        e_gameStates (*event)();
        e_gameStates (*update)();
        void (*render)(float);
        void (*clean)();
    };

    //custom event struct
    struct GAMEEVENT
    {
        union {int mx; uint32_t key{};}; // mx is key in onKeyUp and onKeyDown events
        int my;
        int32_t mxRel; int32_t myRel;
        uint8_t mbutton, type;
        uint8_t winEv;

        GAMEEVENT(): mx(0), my(0), mbutton(0), type(8), mxRel{0}, myRel{0}, winEv{0} {}
    };

    //first 128 keyboard keys, for elegant implementation of repeating
    static bool kbState[128];

    //union for direct access of colors values
    struct COLOR
    {
        union {
            uint32_t color;
            struct {uint8_t a; uint8_t b; uint8_t g; uint8_t r;};
        };
    };


    template <typename T>
    struct lnode
    {
        lnode* pr;
        lnode* nx;
        T data;

        lnode<T>(): pr{nullptr}, nx{nullptr} {}
        ~lnode<T>() {delete data;}
    };

    template <typename T>
    struct list
    {
        lnode<T>* head;
        lnode<T>* tail;

        list<T>() : head{nullptr}, tail{nullptr} {}
        ~list<T>() {clean();}

        bool is_empty() {return !head;}

        T& back() {return tail->data;}

        void append(T&& _data)
        {
            if (!head)
            {
                head = tail = new lnode<T>();
                tail->data = _data;
                return;
            }
            tail->nx = new lnode<T>();
            tail->nx->data = _data;
            tail = tail->nx;
        }

        void clean()
        {
            if (is_empty()) return;
            auto* p = head;
            while (head)
            {
                p = p->nx;
                delete head;
                head = p;
            }
        }

        uint32_t size()
        {
            auto p = head;
            uint32_t size = 0;
            while (p) {p = p->nx; size++;}
            return size;
        }

        void print()
        {
            for (lnode<T>* it = head; it->nx != nullptr; it = it->nx)
            {
                std::cout << it->data << " ";
            }
        }
    };

    /************************************************************/

    void parse_event(SDL_Event* rawEvent, GAMEEVENT* gameEvent);

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

    template <typename T>
    void traverseTrie(lnode<T>* pEl, void (*pFunc)(T))
    {
        pFunc(pEl->data);
        if (!pEl->data->childNodes.is_empty())
        {
            traverseTrie(pEl->data->childNodes.head, pFunc);
        }
        if (pEl->nx)
            traverseTrie(pEl->nx, pFunc);
    }

    long long hashStr(const char* str);
}

//res - all technical outer world resource handling
namespace res
{
    using namespace util;

    //struct FNT
    //{
        //TTF_Font* font;
        //int size;
    //};

    e_exitCodes res_init();
    void res_clean();

    TTF_Font* res_getFont(uint32_t size, uint32_t type);
    uint32_t res_getFontType(const char* name);

    SDL_Surface* res_loadPNG(const char* path);
    TTF_Font* res_loadTTF(const char* path, uint32_t size);
}

//math - all math, used in project (it can be not much, but it still required)
namespace math
{
    //Vec2 struct and related functions
    template <typename T>
    struct v2
    {
        T x; T y;

        explicit v2(T _x, T _y) {x = _x; y = _y;}
        v2() {x = y = 0;}

        inline T norm1() {return fabs(x) + fabs(y);}
        inline T norm2() {return sqrt(x * x + y * y);}
        inline v2<T> rotate(T ang) {return v2<T>{(this->x * cos(ang) - this->y * sin(ang)), (this->x * sin(ang) + this->y * cos(ang))};}

        inline v2<T> operator-() {return v2<T>{-this->x,-this->y};}

        inline v2<T>& operator=(T rhs) {this->x = rhs; this->y = rhs; return *this;}
        inline v2<T>& operator=(const v2<T> rhs) {this->x = rhs.x; this->y = rhs.y; return *this;}
        inline v2<T>& operator+=(const v2<T>&rhs) {this->x += rhs.x; this->y += rhs.y; return *this;}
        inline v2<T>& operator-=(const v2<T>& rhs) {this->x -= rhs.x; this->y -= rhs.y; return *this;}
        inline v2<T>& operator*=(const v2<T>& rhs) {this->x *= rhs.x; this->y *= rhs.y; return *this;} //vector-row multiplication
        inline v2<T>& operator*=(const T rhs) {this->x *= rhs; this->y *= rhs; return *this;}
        template <typename TC>
        inline explicit operator v2<TC>() const {return v2<TC>{static_cast<TC>(this->x), static_cast<TC>(this->y)};}
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

    template <typename T> //vector-row multiplication
    inline v2<T> operator^ (const v2<T>&lhs, const v2<T>&rhs)
    {
        return v2<T>{lhs.x * rhs.x, lhs.y * rhs.y};
    }

    template <typename T> //vector-scalar multiplication
    inline v2<T> operator* (const v2<T>&lhs, const T rhs)
    {
        return v2<T>{lhs.x * rhs, lhs.y * rhs};
    }

    template <typename T> //vector-scalar division
    inline v2<T> operator/ (const v2<T>&lhs, const T rhs)
    {
        constexpr T rhsInv = 1 / rhs;
        return v2<T>{lhs.x * rhsInv, lhs.y * rhsInv};
    }

    template <typename T> //scalar multiplication
    inline T operator* (const v2<T>&lhs, const v2<T>& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const v2<T>& vec)
    {
        os << "X: " << +vec.x << " Y: " << +vec.y << " ";
        return os;
    }

}

namespace gut
{
    SDL_Window* getW();
    SDL_Renderer* getR();
}
#endif //CREN_UTIL_H
