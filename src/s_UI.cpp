//
// Created by uberdever on 19.02.2020.
//

#include <game.hpp>
#include "s_UI.hpp"

namespace ui
{
    util::e_gameStates (*pEventCases[util::SDL_ParseCount + 1])() = {nullptr};

    util::GAMEEVENT* g_event;
    game::GAME_DATA* pd;

    util::hashT<UI::DATA>* pUI_VARS;
    UI::SCENE* pCurUI;
    util::list<UI::ELEMENT*>* dynUI[UI::CONTEXT::GAME_OVER + 1] {nullptr};

    util::e_gameStates (*pEventContexts[UI::CONTEXT::GAME_OVER + 1])(util::lNode<UI::ELEMENT*>*);

    /************************************************************/

    void _convertList(UI::ELEMENT* el)
    {
        if (el->type > UI::STATIC)
        {
            dynUI[game::curUIContext()]->append(el);
        }
    }

    void _defaultUI_VARS(UI::ELEMENT* el)
    {
        switch (el->type) {
            case UI::STATIC: break;
            case UI::BUTTON:
            {
                if (el->btn->type == UI::BUTTON::CHKBOX)
                    *pUI_VARS->search(el->btn->key)->pInt = el->btn->def;
                break;
            }
            case UI::LIST:
            {
                *pUI_VARS->search(el->lst->key)->pInt = el->lst->def;
                break;
            }
            case UI::SLIDER:
                break;
            case UI::LAST:
                break;
        }
    }

    void _processButton(UI::ELEMENT *pEl) //if data is not in table, it will segfault
    {
        switch (pEl->btn->type) {
            case UI::BUTTON::BUTTON_TYPE::CHKBOX:
            {
                (*pEl->texts.back())->isVisible = !pEl->btn->isPressed;
                pEl->btn->isPressed = !*pUI_VARS->search(pEl->btn->key)->pBool;
                *pUI_VARS->search(pEl->btn->key)->pBool = pEl->btn->isPressed;
                break;
            }
            default: std::cout << "Wrong button type" << std::endl; break;
        }
    }

    void _processList(UI::ELEMENT* pEl)
    {
        if (pEl->lst->isPressed)
        {
            int index = 0; auto* it = pEl->texts.head;
            pEl->frame->relSize.y /= pEl->lst->len;
            math::v2<int> pos = math::v2<int>{g_event->mx, g_event->my};
            pos -= pEl->frame->pos;
            pEl->lst->val = pos.y / (pEl->frame->size.y / pEl->lst->len);

            *pUI_VARS->search(pEl->lst->key)->pInt = pEl->lst->val;

            for (it; it && index <= pEl->lst->val; it = it->nx, index++);
            pEl->texts.head->data->text[0] = 0;
            strcat(pEl->texts.head->data->text, it->data->text);
            pEl->texts.head->data->isChanged = true;
        } else
        {
            pEl->frame->relSize.y *= pEl->lst->len;
        }
        pEl->texts.head->data->isVisible = pEl->lst->isPressed;
        for (auto* it = pEl->texts.head->nx; it; it = it->nx) {it->data->isVisible = !pEl->lst->isPressed;}
        for (auto* it = pEl->pics.head->nx; it; it = it->nx) {it->data->isVisible = !pEl->lst->isPressed;}
        pEl->lst->isPressed = !pEl->lst->isPressed;
        traverseTrie(pCurUI->root->childNodes.head, UI::setupChildUI);
    }

    /************************************************************/

    util::e_gameStates _eventMainScreen(util::lNode<UI::ELEMENT*>* pEl) {if (!pEl->data->frame->isVisible) return util::e_gameStates::UI; return util::e_gameStates::UI;}

    util::e_gameStates _eventPause(util::lNode<UI::ELEMENT*>* pEl)
    {
        switch (util::hash(pEl->data->btn->key, strlen(pEl->data->btn->key))) {
            case 696519466: //resume
            {

                SDL_SetRelativeMouseMode(SDL_TRUE);
                game::nextState() = util::GAMEPLAY;
                return util::TEMP;
            }
            case 1445222786: //save
            {
                break;
            }
            case 2443398265: //load
            {
                break;
            }
            case 2870648836: //options
            {
                game::curUIContext() = UI::OPTIONS;
                pCurUI = pd->ui[UI::OPTIONS];
                break;
            }
            case 2905663775: //exit
            {
                game::nextState() = util::EXIT;
                return util::EXIT;
            }
            default: std::cout << "Wrong code <Pause>" << std::endl; break;
        }
        return util::e_gameStates::UI;
    }

    util::e_gameStates _eventOptions(util::lNode<UI::ELEMENT*>* pEl)
    {
        switch (util::hash(pEl->data->btn->key, strlen(pEl->data->btn->key)))
        {
            case 3839375666: //back
            {
                game::curUIContext() = UI::GAME_PAUSE;
                pCurUI = pd->ui[UI::GAME_PAUSE];
                break;
            }
            case 2186233889: //apply
            {
                gut::gut_updWin(global::cfg::resListW()[global::cfg::curChosenRes()], global::cfg::resListH()[global::cfg::curChosenRes()],
                           global::cfg::isFullscreen() ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                break;
            }
            default: std::cout << pEl->data->btn->key << " Wrong code <Options>"; break;
        }
        return util::e_gameStates::UI;
    }

    util::e_gameStates _eventPlayerMenu(util::lNode<UI::ELEMENT*>* pEl)
    {

    }

    /************************************************************/

    util::e_gameStates onExit() { game::nextState() = util::EXIT; return util::EXIT; }
    util::e_gameStates onWindowEvent() {
        return util::e_gameStates::UI;
    }
    util::e_gameStates onKeyDown()
    {
        if ((g_event->key == SDLK_ESCAPE || g_event->key == SDLK_TAB) && game::curUIContext() == UI::PLAYER_MENU)
        {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            game::nextState() = util::GAMEPLAY;
            return util::TEMP;
        }
        if (g_event->key == SDLK_ESCAPE && (game::curUIContext() == UI::GAME_PAUSE || game::curUIContext() == UI::OPTIONS))
        {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            game::nextState() = util::GAMEPLAY;
            return util::TEMP;
        }
        if (g_event->key == SDLK_q)
        {
            game::nextState() = util::EXIT;
            return util::EXIT;
        }
        return util::e_gameStates::UI;
    }
    util::e_gameStates onKeyUp() { return util::e_gameStates::UI; }
    util::e_gameStates onMouseMotion() { return util::e_gameStates::UI; }
    util::e_gameStates onMouseButtonDown()
    {
        auto curCont = game::curUIContext();
        util::e_gameStates state = util::e_gameStates::UI;
        for (util::lNode<UI::ELEMENT*>* it = dynUI[curCont]->tail; it ; it = it->pr) { //We can have overlap, but overlapers must stay at the end of element
            if (util::inField(g_event->mx, g_event->my, it->data->frame->pos.x, it->data->frame->pos.y, it->data->frame->size.x, it->data->frame->size.y))
            {
                if (!it->data->frame->isVisible) return util::e_gameStates::UI;
                switch (it->data->type) {
                    case UI::BUTTON:
                    {
                        if (it->data->btn->type != UI::BUTTON::PUSH) //Overwise, button don't change it's state, so we don't need to process it.
                        {
                            _processButton(it->data);
                            return util::e_gameStates::UI;
                        }
                        break;
                    }
                    case UI::LIST:
                    {
                        _processList(it->data);
                        return util::e_gameStates::UI;
                    }
                    default: break;
                }
                state = pEventContexts[curCont](it);
                break;
            }
        }

        return state;
    }
    util::e_gameStates onMouseButtonUp() { return util::e_gameStates::UI; }
    util::e_gameStates onMouseWheel() { return util::e_gameStates::UI; }
    util::e_gameStates onNothing() { return util::e_gameStates::UI; }

    util::e_gameStates event() {
        SDL_Event rawEvent;
        util::e_gameStates state = util::e_gameStates::UI;

        while (SDL_PollEvent(&rawEvent) && state != util::EXIT)
        {
            util::parse_event(&rawEvent, g_event);
            state = (*pEventCases[g_event->type])();
        }
        return state;
    }

    /************************************************************/

    util::e_gameStates update() {
        return util::e_gameStates::UI;
    }

    /************************************************************/

    void _drawUI(UI::ELEMENT* pEl)
    {
        if (!pEl->frame->isVisible) return;
        const SDL_Rect out {pEl->frame->pos.x, pEl->frame->pos.y, pEl->frame->size.x, pEl->frame->size.y};
        SDL_SetRenderDrawColor(global::renderer(), pEl->frame->col.r, pEl->frame->col.g, pEl->frame->col.b, pEl->frame->col.a);
        SDL_RenderFillRect(global::renderer(), &out);
        SDL_SetRenderDrawColor(global::renderer(), 0, 0, 0, 0);

        util::lNode<UI::PICTURE*>* pIcn = pEl->pics.head;
        while (pIcn)
        {
            if (pIcn->data->isVisible)
            {
                SDL_Rect o {pIcn->data->pos.x, pIcn->data->pos.y, pIcn->data->size.x, pIcn->data->size.y};
                SDL_RenderCopy(global::renderer(), pIcn->data->pic, 0, &o);
            }
            pIcn = pIcn->nx;
        }

        util::lNode<UI::TEXT*>* pText = pEl->texts.head;
        while (pText)
        {
            if (pText->data->isVisible)
            {
                SDL_Rect o {pText->data->pos.x, pText->data->pos.y};
                if (pText->data->isChanged)
                {
                    SDL_Color col = {pText->data->col.r, pText->data->col.g, pText->data->col.b, pText->data->col.a};
                    SDL_Surface* tmpText = TTF_RenderUTF8_Blended(pText->data->fnt, pText->data->text, col);
                    if (pText->data->textTexture)
                    {
                        std::cout << "destroyed" << std::endl;
                        SDL_DestroyTexture(pText->data->textTexture);
                    }
                    pText->data->textTexture = SDL_CreateTextureFromSurface(global::renderer(), tmpText);
                    pText->data->isChanged = false;
                    SDL_FreeSurface(tmpText);
                }
                SDL_QueryTexture(pText->data->textTexture, 0, 0, &o.w, &o.h);
                o.w *= gut::textScaleFactor().x; o.h *= gut::textScaleFactor().y;
                if (pText->data->alignment == 3215930507) {o.x -= o.w / 2; o.y -= o.h / 2;} //center
                else if (pText->data->alignment == 3032534746) {o.x -= o.w;} //left
                SDL_RenderCopy(global::renderer(), pText->data->textTexture, 0, &o);
            }
            pText = pText->nx;
        }
    }

    void render(float) {
        util::traverseTrie<UI::ELEMENT*>(pd->ui[game::curUIContext()]->root->childNodes.head, _drawUI);
        SDL_RenderPresent(global::renderer());
    }

    /************************************************************/

    void bindUI_VARS()
    {
        //(*pUI_VARS)[""].data.pInt = &global::cfg::irrelevantValue();
        (*pUI_VARS)["fullscreen"].data.pBool = &global::cfg::isFullscreen();
        (*pUI_VARS)["resolution"].data.pInt = &global::cfg::curChosenRes();

        for (int i = 0; i < UI::getUI()->size(); i++)
        {
            traverseTrie(pd->ui[i]->root->childNodes.head, _defaultUI_VARS);
        }
    }

    util::e_exitCodes init() {

        pEventCases[0] = onExit;
        pEventCases[1] = onWindowEvent;
        pEventCases[2] = onKeyDown;
        pEventCases[3] = onKeyUp;
        pEventCases[4] = onMouseMotion;
        pEventCases[5] = onMouseButtonDown;
        pEventCases[6] = onMouseButtonUp;
        pEventCases[7] = onMouseWheel;
        pEventCases[8] = onNothing;

        g_event = new util::GAMEEVENT();

        pd = game::getData();

        pCurUI = pd->ui[UI::MAIN_SCREEN];
        for (auto & i : dynUI)
            i = new util::list<UI::ELEMENT*>();
        pUI_VARS = UI::getUI_VARS();

        UI::CONTEXT tmpContext = game::curUIContext();
        for (size_t i = UI::MAIN_SCREEN; i <= UI::GAME_OVER && pd->ui[i]; i++)
        {
            game::curUIContext() = static_cast<UI::CONTEXT>(i);
            traverseTrie(pd->ui[i]->root->childNodes.head, _convertList);
        }
        game::curUIContext() = tmpContext;

        pEventContexts[UI::CONTEXT::MAIN_SCREEN] = _eventMainScreen;
        pEventContexts[UI::CONTEXT::GAME_PAUSE] = _eventPause;
        pEventContexts[UI::CONTEXT::OPTIONS] = _eventOptions;
        pEventContexts[UI::CONTEXT::PLAYER_MENU] = _eventPlayerMenu;

        bindUI_VARS();

        return util::OK;
    }

    /************************************************************/

    void clean() {
        for (auto & i : dynUI)
            delete i;
        delete g_event;
    }
}
