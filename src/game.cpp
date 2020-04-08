//
// Created by uberdever on 18.03.2020.
//
#include "game.hpp"

namespace global
{
    e_gameStates _nextState = e_gameStates::MAIN_MENU;

    /************************************************************/

    e_gameStates& nextState() {
        return _nextState;
    }
}

namespace game
{
    GAME_DATA* gpd; //global pointer for gamedata

    /************************************************************/

    e_exitCodes loadMap(const std::string& name) {
        delete gpd->mp;
        std::string path = "../data/maps/"; path += name;
        std::ifstream in; in.open(path); uint32_t _w, _h;
        if (!in.is_open()) return GAME_MAP_ERR;
        in >> _h; in.get(); in >> _w; in.get(); // '\n'
        gpd->mp = new MAP();
        gpd->mp->id = new uint32_t[_w * _h];
        gpd->mp->w = _w;
        gpd->mp->h = _h;
        for (int i = 0; i < _h; ++i) {
            for (int j = 0; j < _w; ++j) {
                gpd->mp->id[i * _w + j] = in.get();
            }
            in.get(); // '\n'
        }
        in.close();
        return OK;
    }

    //void

    e_exitCodes game_init() {
        e_exitCodes exitCode = OK;
        gpd = new GAME_DATA();
        gpd->pl = new PLAYER();
        gpd->pl->pos.x = 2;
        gpd->pl->pos.y = 2; //TODO delete this after
        exitCode = static_cast<e_exitCodes>(exitCode | loadMap("exam.map"));
        return exitCode;
    }

    void game_clean() {
        delete gpd;
    }

    GAME_DATA* getData() {
        return gpd;
    }


}