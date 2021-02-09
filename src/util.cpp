//
// Created by uberdever on 19.02.2020.
//

#include "util.hpp"

namespace util
{
    void parse_event(SDL_Event* rawEvent, GAMEEVENT* gameEvent)
    {
        static uint32_t eventCodes[SDL_ParseCount] = {SDL_QUIT, SDL_WINDOWEVENT, SDL_KEYDOWN, SDL_KEYUP, SDL_MOUSEMOTION,
                                  SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP, SDL_MOUSEWHEEL};
        uint8_t index = 0;
        for (; index < SDL_ParseCount && (rawEvent->type ^ eventCodes[index]); index++);
        gameEvent->mx = rawEvent->motion.x;
        gameEvent->my = rawEvent->motion.y;
        gameEvent->mxRel = rawEvent->motion.xrel;
        gameEvent->myRel = rawEvent->motion.yrel;
        gameEvent->mbutton = rawEvent->button.button;
        gameEvent->type = index;
        gameEvent->winEv = rawEvent->window.event;
        //std::cout << index << " " << gameEvent->key << " " << gameEvent->my << " " << gameEvent->mx << " " << (uint32_t)gameEvent->mbutton << " " << rawEvent->key.keysym.scancode << std::endl;
    }

    uint32_t hash(const char *data, size_t len) //nginx murmur hash function
    {
        uint32_t  h, k;

        h = 0 ^ len;

        while (len >= 4) {
            k  = data[0];
            k |= data[1] << 8;
            k |= data[2] << 16;
            k |= data[3] << 24;

            k *= 0x5bd1e995;
            k ^= k >> 24;
            k *= 0x5bd1e995;

            h *= 0x5bd1e995;
            h ^= k;

            data += 4;
            len -= 4;
        }

        switch (len) {
            case 3:
                h ^= data[2] << 16;
                /* fall through */
            case 2:
                h ^= data[1] << 8;
                /* fall through */
            case 1:
                h ^= data[0];
                h *= 0x5bd1e995;
        }

        h ^= h >> 13;
        h *= 0x5bd1e995;
        h ^= h >> 15;

        return h;
    }

    bool inField(int mx, int my, int px, int py, int sx, int sy) {
        return ( (mx >= px) && (my >= py) ) && ( (mx <= px + sx) && (my <= py + sy) );
    }

    math::v2<int> parseV2(const char* str)
    {
        char* p;
        int x = strtol(str, &p, 10);
        int y = strtol(p, nullptr, 10);
        return math::v2<int>{x, y};
    }
}


namespace res
{
    /*DATA*/
    /************************************************************/

    util::list<FNT*>* fonts;
    util::vector<SDL_Texture*>* items;

    /*FUNCTIONS*/
    /************************************************************/

    SDL_Surface *res_loadPNG(const char *path) {
        SDL_Surface* tmp = nullptr;
        tmp = IMG_Load(path);
        tmp = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_BGRA32, 0);
        return tmp;
    }

    void res_loadSprite(const char *path)
    {

    }

    TTF_Font* res_loadTTF(const char *path, uint32_t size) {
        TTF_Font* tmp = nullptr;
        tmp = TTF_OpenFont(path, size);
        return tmp;
    }

    TTF_Font* res_getFont(const char* name, uint32_t size) {
        char path[100] = {"../data/fonts/"};
        if (!name)
            return nullptr;
        uint32_t id = util::hash(name, strlen(name));
        for (auto it = fonts->head; it; it = it->nx) {
            if (it->data->size == size && it->data->id == id)
                return it->data->font;
        }
        fonts->append(new FNT());
        (*fonts->back())->font = res_loadTTF(strcat(path, name), size);
        (*fonts->back())->size = size;
        (*fonts->back())->id = id;
        return (*fonts->back())->font;
    }

    /************************************************************/

    util::e_exitCodes res_init()
    {
        //resource setup
        //font part
        fonts = new util::list<FNT*>();
        items = new util::vector<SDL_Texture*>(10);

        //image part

        return util::e_exitCodes::OK;
    }

    void res_clean() {
        fonts->clean_heap();
        delete items;
        delete fonts;
    }
}

/*namespace math
{
    float g_sin[360] = {};
    float g_cos[360] = {};

    void calc_trig(float beta, float* s, float* c)
    {
        constexpr int32_t IT_NUM = 25;
        constexpr int32_t ANG_LEN = 13;

        float angle;
        float angles[ANG_LEN] = {
                7.85398163E-01,
                4.63647609E-01,
                2.44978663E-01,
                1.24354994E-01,
                6.24188099E-02,
                3.12398334E-02,
                1.56237286E-02,
                7.81234106E-03,
                3.90623013E-03,
                1.95312251E-03,
                9.76562189E-04,
                4.88281211E-04,
                2.44140620E-04};


        float kprod = 0.60725293;

        float factor;
        float sign_factor = 1;
        float two_factor;
        float sigma;
        float& sine = *s;
        float& cosine = *c;
        int32_t j;

        beta = beta * util::PI / 180;

        sine = 0;
        cosine = 1.0;

        two_factor = 1.0;
        angle = angles[0];

        for (j = 1; j <= IT_NUM; j++ )
        {
            if ( beta < 0.0 )
            {
                sigma = -1.0;
            }
            else
            {
                sigma = 1.0;
            }

            factor = sigma * two_factor;

            float new_cos =          cosine - factor * sine;
            float new_sin = factor * cosine +          sine;

            cosine = new_cos;
            sine = new_sin;

            beta = beta - sigma * angle;

            two_factor /= 2.0;

            if ( ANG_LEN < j + 1 )
            {
                angle = angle / 2.0f;
            }
            else
            {
                angle = angles[j];
            }
        }


        cosine = cosine * kprod;
        sine = sine * kprod;

        cosine *= sign_factor;
        sine *= sign_factor;
    }

    util::e_exitCodes math_init()
    {
        for (int32_t i = 0; i < 90; i++)
        {
            float s, c;
            calc_trig(i, &s, &c);
            g_sin[i] = s;
            g_cos[i] = c;
            g_sin[i + 90] = c;
            g_cos[i + 90] = -s;
            g_sin[i + 180] = -s;
            g_cos[i + 180] = -c;
            g_sin[i + 270] = -c;
            g_cos[i + 270] = s;
        }

        return util::e_exitCodes::OK;
    }

    float my_sin(float ang) {
        int sign;
        ang > 0 ? sign = 1 : sign = -1, ang = -ang;
        return (float)sign * g_sin[(int)ang];
    }

    float my_cos(float ang) {
        ang = abs(ang);
        return g_cos[(int)ang];
    }
}*/