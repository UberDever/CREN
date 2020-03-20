//
// Created by uberdever on 19.02.2020.
//

#include "s_mainmenu.hpp"

namespace mainmenu
{
    e_gameStates (*pEventCases[SDL_ParseCount + 1])() = {nullptr};

    /************************************************************/

    e_gameStates onExit() { return e_gameStates::EXIT; }
    e_gameStates onWindowEvent() { return e_gameStates::MAIN_MENU; }
    e_gameStates onKeyDown() { return e_gameStates::MAIN_MENU; }
    e_gameStates onKeyUp() { return e_gameStates::MAIN_MENU; }
    e_gameStates onMouseMotion() { return e_gameStates::MAIN_MENU; }
    e_gameStates onMouseButtonDown() { return e_gameStates::MAIN_MENU; }
    e_gameStates onMouseButtonUp() { return e_gameStates::MAIN_MENU; }
    e_gameStates onMouseWheel() { return e_gameStates::MAIN_MENU; }
    e_gameStates onNothing() { return e_gameStates::MAIN_MENU; }

    e_gameStates event() {
        SDL_Event rawEvent;
        SDL_WaitEvent(&rawEvent);
        GameEvent event = parse_event(&rawEvent);
        return (*pEventCases[event.type])();;
    }

    /************************************************************/

    e_gameStates update() {
        return e_gameStates::MAIN_MENU;
    }

    /************************************************************/

    void render() {
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

        return OK;
    }

    /************************************************************/

    void clean() {
    }
}
