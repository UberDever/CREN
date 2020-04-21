//
// Created by uberdever on 19.02.2020.
//

#include "s_pause.hpp"

namespace pause
{
    e_gameStates (*pEventCases[SDL_ParseCount + 1])() = {nullptr};

    GAMEEVENT* g_event;
    GAME_DATA* pd;

    /************************************************************/

    e_gameStates onExit() { return EXIT; }
    e_gameStates onWindowEvent() {
        return PAUSE;
    }
    e_gameStates onKeyDown()
    {
        if (g_event->key == SDLK_ESCAPE)
        {
            //SDL_SetRelativeMouseMode(SDL_TRUE);
            global::nextState() = GAMEPLAY;
            return TEMP;
        }
        if (g_event->key == SDLK_f)
        {
            gut_updWin(800, 600, 0);
        }
        if (g_event->key == SDLK_c)
        {
            gut_updWin(640, 480, 0);
        }
        if (g_event->key == SDLK_g)
        {
            gut_updWin(1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
        if (g_event->key == SDLK_v)
        {
            gut_updWin(640, 480, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }

        return PAUSE;
    }
    e_gameStates onKeyUp() { return PAUSE; }
    e_gameStates onMouseMotion() { return PAUSE; }
    e_gameStates onMouseButtonDown() { return PAUSE; }
    e_gameStates onMouseButtonUp() { return PAUSE; }
    e_gameStates onMouseWheel() { return PAUSE; }
    e_gameStates onNothing() { return PAUSE; }

    e_gameStates event() {
        SDL_Event rawEvent;
        e_gameStates state = PAUSE;

        while (SDL_PollEvent(&rawEvent))
        {
            parse_event(&rawEvent, g_event);
            state = (*pEventCases[g_event->type])();
        }
        return state;
    }

    /************************************************************/

    e_gameStates update() {
        return PAUSE;
    }

    /************************************************************/

    void render(float) {
        gut_UI();
    }

    /************************************************************/

    e_exitCodes init() {

        pEventCases[0] = onExit;
        pEventCases[1] = onWindowEvent;
        pEventCases[2] = onKeyDown;
        pEventCases[3] = onKeyUp;
        pEventCases[4] = onMouseMotion;
        pEventCases[5] = onMouseButtonDown;
        pEventCases[6] = onMouseButtonUp;
        pEventCases[7] = onMouseWheel;
        pEventCases[8] = onNothing;

        g_event = new GAMEEVENT();

        pd = game::getData();

        return OK;
    }

    /************************************************************/

    void clean() {
        delete g_event;
    }
}
