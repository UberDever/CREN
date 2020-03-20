//
// Created by uberdever on 18.03.2020.
//

#ifndef CREN_GAME_HPP
#define CREN_GAME_HPP

#include "util.hpp"

//global - initially for interscene communication
namespace global
{
    using namespace util;

    e_gameStates& nextState();
}

//game - for managing all game related resources
namespace game
{
    using namespace util;

    //useful structs
    struct Player
    {
        //Raycast variables
        v2<float> pos; v2<float> dir; v2<float> plane;

        Player(): pos{0,0}, dir{-1, 0}, plane{0, 0.666666f} {}

        inline float mapAngle()
        {
            return 0;
        }
    };

    /************************************************************/

    Player& getPlayer();

    e_exitCodes game_init();
    void game_clean();
}

#endif //CREN_GAME_HPP
