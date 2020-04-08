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

    e_gameStates _onExit() { return EXIT; }
    e_gameStates _onWindowEvent() { return GAMEPLAY; }

    e_gameStates _onKeyDown()
    {
        if (g_event->key < 128)
            kbState[g_event->key] = true;
        if (g_event->key == SDLK_ESCAPE)
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            global::nextState() = MAIN_MENU;
            return TEMP;
        }
        return GAMEPLAY;
    }

    e_gameStates _onKeyUp()
    {
        if (g_event->key < 128)
            kbState[g_event->key] = false;
        return GAMEPLAY;
    }
    e_gameStates _onMouseMotion()
    {
        pd->pl->dir = pd->pl->dir.rotate(g_event->mxRel * pd->pl->rotSpeed);
        pd->pl->plane = pd->pl->plane.rotate(g_event->mxRel * pd->pl->rotSpeed);
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
        pd->pl->momentum = pd->pl->dir * (float)(kbState[SDLK_w] - kbState[SDLK_s]) //accounting to all directions (dir and plane)
                           + pd->pl->plane * (float)(kbState[SDLK_d] - kbState[SDLK_a]);
        pd->pl->pos += pd->pl->momentum * pd->pl->speed;
        return GAMEPLAY;
    }

    /************************************************************/

    void render(float) {
        gut_raycast();
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

