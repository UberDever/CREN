//
// Created by uberdever on 18.02.2020.
//

#include "s_gameplay.hpp"

namespace gameplay
{
    DATA* pd;
    e_gameStates (*pEventCases[SDL_ParseCount + 1])() = {nullptr};

    /************************************************************/

    e_gameStates _onExit() { return e_gameStates::EXIT; }
    e_gameStates _onWindowEvent() { return e_gameStates::GAMEPLAY; }
    e_gameStates _onKeyDown() { return e_gameStates::GAMEPLAY; }
    e_gameStates _onKeyUp() { return e_gameStates::GAMEPLAY; }
    e_gameStates _onMouseMotion() { return e_gameStates::GAMEPLAY; }
    e_gameStates _onMouseButtonDown() { return e_gameStates::GAMEPLAY; }
    e_gameStates _onMouseButtonUp() { return e_gameStates::GAMEPLAY; }
    e_gameStates _onMouseWheel() { return e_gameStates::GAMEPLAY; }
    e_gameStates _onNothing() { return e_gameStates::GAMEPLAY; }

    e_gameStates event() {
        SDL_Event rawEvent;
        SDL_WaitEvent(&rawEvent);
        c_gameEvent event = parse_event(&rawEvent);
        return (*pEventCases[event.type])();;
    }

    /************************************************************/

    e_gameStates update() {
        return e_gameStates::GAMEPLAY;
    }

    /************************************************************/

    void render() {
        gut_draw();
    }

    /************************************************************/

    e_exitCodes init() {
        DATA* pd = new DATA();

        pEventCases[0] = _onExit;
        pEventCases[1] = _onWindowEvent;
        pEventCases[2] = _onKeyDown;
        pEventCases[3] = _onKeyUp;
        pEventCases[4] = _onMouseMotion;
        pEventCases[5] = _onMouseButtonDown;
        pEventCases[6] = _onMouseButtonUp;
        pEventCases[7] = _onMouseWheel;
        pEventCases[8] = _onNothing;

        return static_cast<e_exitCodes>(e_exitCodes::OK | !pd);
    }

    /************************************************************/

    void clean() {
        delete pd;
    }
}

