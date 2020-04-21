//
// Created by uberdever on 19.02.2020.
//

#include "cren.hpp"

namespace cren
{
    //game cycle vars
    e_gameStates curState = PAUSE;
    SCENE* p_Scene[SCENE_NUM] = {nullptr};
    constexpr uint32_t TICKS = 50;
    constexpr uint32_t SKIP_TIME = 1000 / TICKS;
    constexpr uint32_t FRAMESKIP = 5;

    //main block (definition)
    e_exitCodes exec()
    {
        e_exitCodes exitcode;
        if ((exitcode = init()) != OK)
            return exitcode;

        uint32_t time = SDL_GetTicks();
        uint32_t cycles = 0;
        float interpolation;

        while (curState != EXIT)
        {
            cycles = 0;
            while (SDL_GetTicks() > time && cycles < FRAMESKIP)
            {
                curState = global::nextState(); //used for changing state on next iteration of cycle
                curState = p_Scene[curState]->event();
                curState = p_Scene[curState]->update();

                time += SKIP_TIME;
                cycles++;
            }

            interpolation = (float)(SDL_GetTicks() + SKIP_TIME - time) / (float)(SKIP_TIME);

            p_Scene[curState]->render(interpolation);
        }

        clean();

        return OK;
    }

    e_exitCodes init()
    {
        e_exitCodes exitCode = OK;

        //exitCode = static_cast<e_exitCodes>(exitCode | math_init());
        exitCode = static_cast<e_exitCodes>(exitCode | gut_init());
        exitCode = static_cast<e_exitCodes>(exitCode | res_init());
        exitCode = static_cast<e_exitCodes>(exitCode | UI_init());


        exitCode = static_cast<e_exitCodes>(exitCode | game_init());


        s_init();

        return exitCode;
    }

    void clean() {

        res_clean();
        gut_clean();
        game_clean();
        UI_clean();

        for (auto & i : p_Scene) {
            delete i;
        }
    }

    void s_init() {
        p_Scene[EXIT] = new SCENE();
        p_Scene[EXIT]->init_scene(Void::init, Void::event, Void::update, Void::render, Void::clean);
        p_Scene[MAIN_MENU] = new SCENE();
        p_Scene[MAIN_MENU]->init_scene(mainmenu::init, mainmenu::event, mainmenu::update, mainmenu::render, mainmenu::clean);
        p_Scene[GAMEPLAY] = new SCENE();
        p_Scene[GAMEPLAY]->init_scene(gameplay::init, gameplay::event, gameplay::update, gameplay::render, gameplay::clean);
        p_Scene[PAUSE] = new SCENE();
        p_Scene[PAUSE]->init_scene(pause::init, pause::event, pause::update, pause::render, pause::clean);
        p_Scene[TEMP] = new SCENE();
        p_Scene[TEMP]->init_scene(temp::init, temp::event, temp::update, temp::render, temp::clean);
    }
}