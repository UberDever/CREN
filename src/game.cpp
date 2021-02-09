//
// Created by uberdever on 18.03.2020.
//
#include "game.hpp"

namespace game
{
    GAME_DATA* gpd; //global pointer for gamedata

    util::e_gameStates _nextState = util::e_gameStates::UI;
    UI::CONTEXT _curUIContext = UI::CONTEXT::PLAYER_MENU;

    /************************************************************/

    util::e_exitCodes loadMap(const std::string& name) {
        delete gpd->mp;
        std::string path = "../data/maps/"; path += name;
        std::ifstream in; in.open(path); uint32_t _w, _h;
        if (!in.is_open()) return util::GAME_MAP_ERR;
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
        return util::OK;
    }

    util::e_exitCodes game_init() {
        util::e_exitCodes exitCode = util::OK;

        gpd = new GAME_DATA();
        gpd->pl = new PLAYER();
        gpd->pl->inv = new items::INV(global::cfg::pl_invW(), global::cfg::pl_invH());

        gpd->ui = new UI::SCENE*[UI::CONTEXT::GAME_OVER + 1] {nullptr};
        gpd->ui_vars = UI::getUI_VARS();
        util::lNode<UI::SCENE*>* pSc = UI::getUI()->head;
        for (int i = UI::MAIN_SCREEN; pSc && i <= UI::GAME_OVER; ++i, pSc = pSc->nx) {gpd->ui[i] = pSc->data;}

        //for (int i = 0; i < 5; i++){
        //    BASE_ITEM item = constructItem(99, static_cast<ITEM_TYPES>(THING | OHWEAP | CHEST | HELM)); //THING | OHWEAP | THWEAP |
        //    //if (item.quality == UNIQUE) std::cout << "UNIQUE: ";
        //    //else if (item.quality == ENCHANTED) std::cout << "ENCHANTED: ";
        //    //else if (item.quality == SPECIAL) std::cout << "SPECIAL: ";
        //    //else {std::cout << "COMMON: ";};
        //    std::cout << item.name << std::endl;
        //    for (auto& it = item.attrs.head; it; it=it->nx)
        //    {
        //        //std::cout << it->data.desc << std::endl;
        //    }
        //    gpd->pl->inv->add(item);
        //}
        //gpd->pl->inv->print();
//
        //std::cout << gpd->pl->inv->get(4, 0).name << std::endl;

        gpd->pl->pos.x = 2;
        gpd->pl->pos.y = 2; //TODO delete this after

        exitCode = static_cast<util::e_exitCodes>(exitCode + loadMap("exam.map"));
        return exitCode;
    }

    void game_clean() {
        delete gpd;
    }

    GAME_DATA* getData() {
        return gpd;
    }

    util::e_gameStates& nextState() {
        return _nextState;
    }

    UI::CONTEXT &curUIContext() {
        return _curUIContext;
    }

}