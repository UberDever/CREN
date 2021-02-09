//
// Created by uberdever on 19.02.2020.
//

#include "s_pause.hpp"

namespace pause
{
    e_gameStates (*pEventCases[SDL_ParseCount + 1])() = {nullptr};

    GAMEEVENT* g_event;
    GAME_DATA* pd;

    hashT<size_t>* pUI_VARS;
    UI_SCENE* pCurUI;
    list<UI_ELEMENT*>* dynUI;

    /************************************************************/

    void _convertList(UI_ELEMENT* el)
    {
        if (el->type > DYNAMIC)
        {
            dynUI->append(el);
        }
    }

    e_gameStates onExit() {global::nextState() = EXIT; return EXIT; }
    e_gameStates onWindowEvent() {
        return PAUSE;
    }
    e_gameStates onKeyDown()
    {
        if (g_event->key == SDLK_ESCAPE)
        {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            global::nextState() = GAMEPLAY;
            return TEMP;
        }
        if (g_event->key == SDLK_f)
        {
            gut_updWin(800, 600, 0);
        }
        if (g_event->key == SDLK_c)
        {
            gut_updWin(640, 480, 0);
        }
        if (g_event->key == SDLK_g)
        {
            gut_updWin(1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
        if (g_event->key == SDLK_v)
        {
            gut_updWin(640, 480, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }

        return PAUSE;
    }
    e_gameStates onKeyUp() { return PAUSE; }
    e_gameStates onMouseMotion() { return PAUSE; }
    e_gameStates onMouseButtonDown()
    {
        for (auto* it = dynUI->head; it ; it = it->nx) {
            if (inField(g_event->mx, g_event->my, it->data->frame->pos.x, it->data->frame->pos.y, it->data->frame->size.x, it->data->frame->size.y))
            {
                //switch(it->data->frame->type)
                _processButton(it->data);
                char str[20] = {};
                //std::cout << hash(it->data->btn->key, strlen(it->data->btn->key)) << std::endl;
                switch (hash(it->data->btn->key, strlen(it->data->btn->key))) {
                    case 432208689: //resume
                    {
                        SDL_SetRelativeMouseMode(SDL_TRUE);
                        global::nextState() = GAMEPLAY;
                        return TEMP;
                    }
                    case 854449289: //save
                    {
                        break;
                    }
                    case 2284594140: //load
                    {
                        break;
                    }
                    case 2749724655: //options
                    {
                        global::curUIContext() = OPTIONS;
                        pCurUI = pd->ui[UI::OPTIONS];
                        dynUI->clean();
                        traverseTrie(pCurUI->root->childNodes.head, _convertList);
                        break;
                    }
                    case 1844252986: //exit
                    {
                        global::nextState() = EXIT;
                        return EXIT;
                    }
                    default: break;
                }
                //std::cout << (*pUI_VARS).search("exit", it->data->btn->val) << std::endl;
            }
        }

        return PAUSE;
    }
    e_gameStates onMouseButtonUp() { return PAUSE; }
    e_gameStates onMouseWheel() { return PAUSE; }
    e_gameStates onNothing() { return PAUSE; }

    e_gameStates event() {
        SDL_Event rawEvent;
        e_gameStates state = PAUSE;

        while (SDL_PollEvent(&rawEvent))
        {
            parse_event(&rawEvent, g_event);
            state = (*pEventCases[g_event->type])();
        }
        return state;
    }

    /************************************************************/

    //e_gameStates _update

    e_gameStates update() {
        /*static const size_t titlesCount = 5;
        size_t irrelevantValue;
        static char* btnTitles[titlesCount] = {"resume", "save", "load", "options", "exit"};
        for (size_t i = 0; i < titlesCount; i++)
        {
            if (pUI_VARS->search(btnTitles[i], irrelevantValue))
            {
                //pCurUI = pd->ui[UI::UI_CONTEXT::]
                return EXIT;
            }
        }*/
        return PAUSE;
    }

    /************************************************************/

    void render(float) {
        gut_UI();
    }

    /************************************************************/

    e_exitCodes init() {

        pEventCases[0] = onExit;
        pEventCases[1] = onWindowEvent;
        pEventCases[2] = onKeyDown;
        pEventCases[3] = onKeyUp;
        pEventCases[4] = onMouseMotion;
        pEventCases[5] = onMouseButtonDown;
        pEventCases[6] = onMouseButtonUp;
        pEventCases[7] = onMouseWheel;
        pEventCases[8] = onNothing;

        g_event = new GAMEEVENT();

        pd = game::getData();

        pCurUI = pd->ui[UI::GAME_PAUSE];
        dynUI = new list<UI_ELEMENT*>();
        pUI_VARS = UI::getUI_VARS();

        traverseTrie(pCurUI->root->childNodes.head, _convertList);

        return OK;
    }

    /************************************************************/

    void clean() {
        delete dynUI;
        delete g_event;
    }
}
