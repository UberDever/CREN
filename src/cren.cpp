//
// Created by uberdever on 19.02.2020.
//

#include "cren.hpp"

namespace cren
{
    //game cycle vars
    e_gameStates curState = e_gameStates::GAMEPLAY;
    Scene* p_Scene[SCENE_NUM] = {nullptr};

    //main block (definition)
    e_exitCodes exec()
    {
        e_exitCodes exitcode;
        if ((exitcode = init()) != e_exitCodes::OK)
            return exitcode;
        while (curState != e_gameStates::EXIT)
        {
            e_gameStates newStateE = p_Scene[curState]->event();
            e_gameStates newStateU = p_Scene[curState]->update();
            p_Scene[curState]->render();
            curState = (newStateE ^ curState) ? newStateE : newStateU;
        }

        clean();

        return e_exitCodes::OK;
    };

    e_exitCodes init()
    {
        e_exitCodes exitCode = e_exitCodes::OK;
        exitCode = static_cast<e_exitCodes>(exitCode | math_init());
        exitCode = static_cast<e_exitCodes>(exitCode | gut_init());
        exitCode = static_cast<e_exitCodes>(exitCode | res_init());

        s_init();

        return exitCode;
    }

    void clean() {

        res_clean();
        gut_clean();

        for (int i = 0; i < SCENE_NUM; ++i) {
            delete p_Scene[i];
        }
    }

    void s_init() {
        p_Scene[EXIT] = new Scene();
        p_Scene[EXIT]->init_scene(Void::init, Void::event, Void::update, Void::render, Void::clean);
        p_Scene[MAIN_MENU] = new Scene();
        p_Scene[MAIN_MENU]->init_scene(mainmenu::init, mainmenu::event, mainmenu::update, mainmenu::render, mainmenu::clean);
        p_Scene[GAMEPLAY] = new Scene();
        p_Scene[GAMEPLAY]->init_scene(gameplay::init, gameplay::event, gameplay::update, gameplay::render, gameplay::clean);
    }
}