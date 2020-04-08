//
// Created by uberdever on 18.03.2020.
//

#ifndef CREN_GAME_HPP
#define CREN_GAME_HPP

#include "util.hpp"
#include "UI.hpp"
#include "gut.hpp"

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
    using namespace math;
    using namespace UI;

    /*Game structs declarations*/

    struct MAP
    {
        uint32_t* id; //map of tiles id
        uint32_t w, h; //map dimensions

        MAP() : id{nullptr}, w{0}, h{0} {}
    };

    //useful structs
    struct PLAYER
    {
        //Raycast and positioning variables
        v2<float> pos; v2<float> dir; v2<float> plane; v2<float> momentum; float rotSpeed;  float speed;

        PLAYER():
                pos{0,0}, dir{1.f, 0}, plane{0, cfg::getCFG().pl_planeSize}, momentum{0, 0}, speed{0.2f}, rotSpeed{PI / 180 * cfg::getCFG().pl_rotSpeed}
        {}

        inline float mapAngle()
        {
            return 0;
        }
    };

    //Global struct, for data unification
    struct GAME_DATA
    {
        PLAYER* pl;
        MAP* mp;
        UI_SCENE* ui; //pointer for current UI context

        GAME_DATA() : pl{nullptr}, mp{nullptr}, ui{nullptr} {}
        ~GAME_DATA()
        {
            delete pl;
            delete mp;
            delete ui;
        }
    };

    GAME_DATA* getData();

    e_exitCodes game_init();
    void game_clean();
}

#endif //CREN_GAME_HPP
