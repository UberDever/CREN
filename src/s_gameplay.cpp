//
// Created by uberdever on 18.02.2020.
//

#include "s_gameplay.hpp"

namespace gameplay
{
    DATA* pd;
    e_exitCodes init() {
        DATA* pd = new DATA();
        return static_cast<e_exitCodes>(e_exitCodes::OK || !pd);
    }

    e_gameStates event() {
        SDL_Event rawEvent;
        SDL_WaitEvent(&rawEvent);
        c_gameEvent event = parse_event(&rawEvent);

        return e_gameStates::GAMEPLAY;
    }

    e_gameStates update() {
        return e_gameStates::GAMEPLAY;
    }

    void render() {
    }

    void clean() {
        delete pd;
    }
}

