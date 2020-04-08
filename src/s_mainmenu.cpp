//
// Created by uberdever on 19.02.2020.
//

#include "s_mainmenu.hpp"

namespace mainmenu
{
    e_gameStates (*pEventCases[SDL_ParseCount + 1])() = {nullptr};

    GAMEEVENT* g_event;
    GAME_DATA* pd;

    /************************************************************/

    e_gameStates onExit() { return EXIT; }
    e_gameStates onWindowEvent() { return MAIN_MENU; }
    e_gameStates onKeyDown()
    {
        if (g_event->key == SDLK_ESCAPE)
        {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            global::nextState() = GAMEPLAY;
            return TEMP;
        }
        
        return MAIN_MENU;
    }
    e_gameStates onKeyUp() { return MAIN_MENU; }
    e_gameStates onMouseMotion() { return MAIN_MENU; }
    e_gameStates onMouseButtonDown() { return MAIN_MENU; }
    e_gameStates onMouseButtonUp() { return MAIN_MENU; }
    e_gameStates onMouseWheel() { return MAIN_MENU; }
    e_gameStates onNothing() { return MAIN_MENU; }

    e_gameStates event() {
        SDL_Event rawEvent;
        e_gameStates state = MAIN_MENU;

        while (SDL_PollEvent(&rawEvent))
        {
            parse_event(&rawEvent, g_event);
            state = (*pEventCases[g_event->type])();
        }
        return state;
    }

    /************************************************************/

    e_gameStates update() {
        return MAIN_MENU;
    }

    /************************************************************/

    void render(float) {
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
