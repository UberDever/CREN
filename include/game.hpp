//
// Created by uberdever on 18.03.2020.
//

#ifndef CREN_GAME_HPP
#define CREN_GAME_HPP

#include "gut.hpp"
#include "UI.hpp"
#include "items.hpp"

//game - for managing all game related resources
namespace game
{
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
        items::INV* inv;

        //Raycast and positioning variables
        math::v2<float> pos; math::v2<float> dir; math::v2<float> plane; math::v2<float> momentum; float rotSpeed;  float speed; int offset; int off;

        PLAYER():
                pos{0,0}, dir{0, 1.f}, plane{-global::cfg::pl_planeSize(), 0}, momentum{0, 0}, speed{0.2f}, rotSpeed{math::PI / 180 * global::cfg::pl_rotSpeed()},
                inv{nullptr},
                offset{0}, off{0}
        {}

        inline float FOV() const
        {
            return atan(plane.y / dir.x) / math::PI * 360;
        }

        inline float mapAngle()
        {
            return 0;
        }
    };

    //Global struct, for data unification (it's ETERNAL, deleted at the end of the program)
    struct GAME_DATA
    {
        PLAYER* pl;
        MAP* mp;
        UI::SCENE** ui; //array of pointers for ui scenes
        util::hashT<UI::DATA> *ui_vars;

        GAME_DATA() : pl{nullptr}, mp{nullptr}, ui{nullptr}, ui_vars{nullptr} {}
        ~GAME_DATA()
        {
            delete pl;
            delete mp;
            delete[] ui; //delete array of pointers, contains handled by ui
        }
    };

    GAME_DATA* getData();

    util::e_gameStates& nextState();
    UI::CONTEXT& curUIContext();

    util::e_exitCodes game_init();
    void game_clean();
}

#endif //CREN_GAME_HPP
