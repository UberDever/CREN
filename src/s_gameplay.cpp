//
// Created by uberdever on 18.02.2020.
//

#include "s_gameplay.hpp"

namespace gameplay
{
    e_gameStates (*pEventCases[SDL_ParseCount + 1])() = {nullptr};

    /************************************************************/

    e_gameStates _onExit() { return EXIT; }
    e_gameStates _onWindowEvent() { return GAMEPLAY; }
    e_gameStates _onKeyDown() { return GAMEPLAY; }
    e_gameStates _onKeyUp() { return GAMEPLAY; }
    e_gameStates _onMouseMotion() { return GAMEPLAY; }
    e_gameStates _onMouseButtonDown() { return GAMEPLAY; }
    e_gameStates _onMouseButtonUp() { return GAMEPLAY; }
    e_gameStates _onMouseWheel() { return GAMEPLAY; }
    e_gameStates _onNothing() { return GAMEPLAY; }

    e_gameStates event() {
        SDL_Event rawEvent;
        SDL_WaitEvent(&rawEvent);
        GameEvent event = parse_event(&rawEvent);
        return (*pEventCases[event.type])();;
    }

    /************************************************************/

    e_gameStates update() {
        return GAMEPLAY;
    }

    /************************************************************/

    void render() {
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

        v2<double> vec1  {0.1, 0.5};
        v2<double> vec2  {4.2, 3.6};
        std::cout << vec1.norm2() << '\n' << vec1 << '\n' << vec2 << std::endl;

        return OK;
    }

    /************************************************************/

    void clean() {
    }
}

