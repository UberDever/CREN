//
// Created by uberdever on 18.02.2020.
//

#include "s_gameplay.hpp"

namespace gameplay
{
    e_gameStates (*pEventCases[SDL_ParseCount + 1])() = {nullptr};

    GAMEEVENT* g_event;
    GAME_DATA* pd;

    /************************************************************/

    e_gameStates _onExit() { game::nextState() = EXIT; return EXIT; }
    e_gameStates _onWindowEvent() { return GAMEPLAY; }

    e_gameStates _onKeyDown()
    {
        if (g_event->scancode < 128)
            kbState[g_event->scancode] = true;
        if (g_event->key == SDLK_ESCAPE)
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            game::nextState() = e_gameStates::UI;
            game::curUIContext() = UI::CONTEXT::GAME_PAUSE;
            for (bool & i : kbState) i = false;
            return TEMP;
        }
        if (g_event->key == SDLK_TAB)
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            game::nextState() = e_gameStates::UI;
            game::curUIContext() = UI::CONTEXT::PLAYER_MENU;
            for (bool & i : kbState) i = false;
            return TEMP;
        }
        return GAMEPLAY;
    }

    e_gameStates _onKeyUp()
    {
        if (g_event->scancode < 128)
            kbState[g_event->scancode] = false;
        return GAMEPLAY;
    }
    e_gameStates _onMouseMotion()
    {
        pd->pl->dir = pd->pl->dir.rotate(g_event->mxRel * pd->pl->rotSpeed);
        pd->pl->plane = pd->pl->plane.rotate(g_event->mxRel * pd->pl->rotSpeed);
        pd->pl->offset += g_event->myRel;
        return GAMEPLAY;
    }
    e_gameStates _onMouseButtonDown() { return GAMEPLAY; }
    e_gameStates _onMouseButtonUp() { return GAMEPLAY; }
    e_gameStates _onMouseWheel() { return GAMEPLAY; }
    e_gameStates _onNothing() { return GAMEPLAY; }


    e_gameStates event() {
        SDL_Event rawEvent;
        e_gameStates state = e_gameStates::GAMEPLAY;

        while (SDL_PollEvent(&rawEvent))
        {
            parse_event(&rawEvent, g_event);
            state = (*pEventCases[g_event->type])();
        }
        return state;
    }

    /************************************************************/

    e_gameStates update() {
        pd->pl->momentum = pd->pl->dir * (float)(kbState[SDL_SCANCODE_W] - kbState[SDL_SCANCODE_S]) //accounting to all directions (dir and plane)
                           + pd->pl->plane * (float)(kbState[SDL_SCANCODE_D] - kbState[SDL_SCANCODE_A]);
        pd->pl->pos += pd->pl->momentum * pd->pl->speed;
        return GAMEPLAY;
    }

    /************************************************************/

    void render(float) { //TODO: REBUILD
        auto* map = pd->mp->id;
        int scrW = global::cfg::scrRenW();
        int scrH = global::cfg::scrRenH();

        int dS = 0;
        int dE = 0;

        gut::cls();

        for (int x = 0; x < scrW; ++x) {
            float cam = 2.f * (float)x / (float)scrW - 1;
            math::v2<float> ray = pd->pl->dir + (pd->pl->plane * cam);
            math::v2<int> pos {static_cast<int>(pd->pl->pos.x), static_cast<int>(pd->pl->pos.y)};
            math::v2<float> side;
            math::v2<float> delta {std::fabs(1 / ray.x), std::fabs(1 / ray.y)};
            //v2<float> delta {(ray.y == 0) ? 0 : ((ray.x == 0) ? 1 : std::fabs(1 / ray.x)), (ray.x == 0) ? 0 : ((ray.y == 0) ? 1 : std::fabs(1 / ray.y))};
            float pwd = 0;
            math::v2<int> step;
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

            int line = (int)(global::cfg::mp_wallHeight() * (float)scrH / pwd);
            if (line) //for evading zero-length lines
            {
                dS = -line / 2 + (scrH + pd->pl->off) / 2 - pd->pl->offset; dS = dS < 0 ? 0 : dS;
                dE = line / 2 + (scrH + pd->pl->off) / 2 - pd->pl->offset; dE = dE >= scrH ? scrH - 1 : dE;
                //dS = -line / 2 + scrH / 2 - pd->pl->offset; dS = dS < 0 ? 0 : dS;
                //dE = line / 2 + scrH  / 2 - pd->pl->offset; dE = dE >= scrH ? scrH - 1 : dE;
            }

            for (int i = dS; i < dE; ++i) {
                gut::setPixel(x, i * scrW);
            }
        }
        SDL_Rect o {0, 0, global::cfg::scrUIW(), global::cfg::scrUIH() / 2};
        SDL_UpdateTexture(global::gameScreen(), 0, global::gameScreenBuffer(), sizeof(uint32_t) * global::cfg::scrRenW());
        //SDL_RenderCopy(global::renderer(), skybox, 0, &o);
        SDL_RenderCopy(global::renderer(), global::gameScreen(), 0, 0);
        SDL_RenderPresent(global::renderer());
    }

    /************************************************************/

    e_exitCodes init() {

        pEventCases[0] = _onExit;
        pEventCases[1] = _onWindowEvent;
        pEventCases[2] = _onKeyDown;
        pEventCases[3] = _onKeyUp;
        pEventCases[4] = _onMouseMotion;
        pEventCases[5] = _onMouseButtonDown;
        pEventCases[6] = _onMouseButtonUp;
        pEventCases[7] = _onMouseWheel;
        pEventCases[8] = _onNothing;

        g_event = new GAMEEVENT();

        pd = game::getData();

        return OK;
    }

    /************************************************************/

    void clean() {
        delete g_event;
    }
}

