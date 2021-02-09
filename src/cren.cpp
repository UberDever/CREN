//
// Created by uberdever on 19.02.2020.
//

#include "cren.hpp"

namespace cren
{
    //game cycle vars
    e_gameStates curState = e_gameStates::UI;
    SCENE* p_Scene[TEMP] = {nullptr};
    constexpr uint32_t TICKS = 60;
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
            while (SDL_GetTicks() > time && cycles < FRAMESKIP && curState != EXIT)
            {
                curState = game::nextState(); //used for changing state on next iteration of cycle
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
        std::srand(unsigned(time(0)));
        exitCode = static_cast<e_exitCodes>(exitCode + gut::gut_init());
        exitCode = static_cast<e_exitCodes>(exitCode + res::res_init());
        exitCode = static_cast<e_exitCodes>(exitCode + UI::UI_init());
        exitCode = static_cast<e_exitCodes>(exitCode + items::items_init());

        exitCode = static_cast<e_exitCodes>(exitCode + game::game_init());


        s_init();

        return exitCode;
    }

    void clean() {
        res::res_clean();
        gut::gut_clean();
        game::game_clean();
        UI::UI_clean();
        items::items_clean();

        for (auto & i : p_Scene) {
            delete i;
        }
    }

    void s_init() {
        p_Scene[EXIT] = new SCENE();
        p_Scene[EXIT]->init_scene(Void::init, Void::event, Void::update, Void::render, Void::clean);
        p_Scene[GAMEPLAY] = new SCENE();
        p_Scene[GAMEPLAY]->init_scene(gameplay::init, gameplay::event, gameplay::update, gameplay::render, gameplay::clean);
        p_Scene[e_gameStates::UI] = new SCENE();
        p_Scene[e_gameStates::UI]->init_scene(ui::init, ui::event, ui::update, ui::render, ui::clean);
        p_Scene[TEMP] = new SCENE();
        p_Scene[TEMP]->init_scene(temp::init, temp::event, temp::update, temp::render, temp::clean);
    }
}