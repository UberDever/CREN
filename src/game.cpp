//
// Created by uberdever on 18.03.2020.
//
#include "game.hpp"

namespace global
{
    e_gameStates _nextState = e_gameStates::GAMEPLAY;

    /************************************************************/

    e_gameStates& nextState() {
        return _nextState;
    }
}

namespace game
{
    Player* g_Player = nullptr;

    /************************************************************/

    Player& getPlayer() {return *g_Player;}

    e_exitCodes game_init() {

        g_Player = new Player();

        return OK;
    }

    void game_clean() {
        delete g_Player;
    }


}