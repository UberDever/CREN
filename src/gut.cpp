//
// Created by uberdever on 20.02.2020.
//

#include "gut.hpp"

namespace gut
{
    void (*pDrawUI[UI_TYPES::LAST + 1])(UI_ELEMENT*) = {nullptr};

    SDL_Window* g_wind;
    SDL_Renderer* g_rend;
    SDL_Texture* g_text;

    uint32_t* g_textBuf;

    //SDL_Surface* gameSurf = nullptr;
    //SDL_Surface* temp_surf = nullptr; //for global use in gut functions

    /************************************************************/

    SDL_Window* getW() { return g_wind; } //declared in util, for global use
    SDL_Renderer* getR() { return g_rend; }

    /************************************************************/

    void gut_raycast() //TODO: REBUILD
    {
        static game::GAME_DATA* pd = game::getData();

        auto* map = pd->mp->id;
        int scrW = getCFG().scrRenW;
        int scrH = getCFG().scrRenH;

        int dS = 0;
        int dE = 0;

        memset(g_textBuf, 0, scrW * scrH * sizeof(uint32_t));

        for (int x = 0; x < scrW; ++x) {
            float cam = 2.f * (float)x / (float)scrW - 1;
            v2<float> ray = pd->pl->dir + (pd->pl->plane * cam);
            v2<int> pos {static_cast<int>(pd->pl->pos.x), static_cast<int>(pd->pl->pos.y)};
            v2<float> side;
            v2<float> delta {std::fabs(1 / ray.x), std::fabs(1 / ray.y)};
            //v2<float> delta {(ray.y == 0) ? 0 : ((ray.x == 0) ? 1 : std::fabs(1 / ray.x)), (ray.x == 0) ? 0 : ((ray.y == 0) ? 1 : std::fabs(1 / ray.y))};
            float pwd = 0;
            v2<int> step;
            bool hit = false, NS = true; //North-South

            if (ray.x > 0)
            {
                step.x = 1;
                side.x = ((float)pos.x + 1 - pd->pl->pos.x) * delta.x;
            } else
            {
                step.x = -1;
                side.x = (pd->pl->pos.x - pos.x) * delta.x;
            }
            if (ray.y > 0)
            {
                step.y = 1;
                side.y = ((float)pos.y + 1 - pd->pl->pos.y) * delta.y;
            } else
            {
                step.y = -1;
                side.y = (pd->pl->pos.y - pos.y) * delta.y;
            }
            while (!hit)
            {
                if (side.x < side.y)
                {
                    side.x += delta.x;
                    pos.x += step.x;
                    NS = true;
                } else
                {
                    side.y += delta.y;
                    pos.y += step.y;
                    NS = false;
                }
                if (map[pos.y * pd->mp->w + pos.x] == '1') hit = true;
            }

            if (NS) pwd = (pos.x - pd->pl->pos.x + (1.f - step.x) / 2) / ray.x;
            else    pwd = (pos.y - pd->pl->pos.y + (1.f - step.y) / 2) / ray.y;

            int line = (int)(getCFG().mp_wallHeight * (float)scrH / pwd);
            if (line) //for evading zero-length lines
            {
                dS = -line / 2 + scrH / 2; dS = dS < 0 ? 0 : dS;
                dE = line / 2 + scrH / 2; dE = dE >= scrH ? scrH - 1 : dE;
            }

            for (int i = dS; i < dE; ++i) {
                g_textBuf[i * scrW + x] = 0x0000FFFF;
            }
        }
        SDL_UpdateTexture(g_text, nullptr, &g_textBuf[0], scrW * sizeof(uint32_t));
        SDL_RenderCopy(g_rend, g_text, nullptr, nullptr);
        SDL_RenderPresent(g_rend);
    }

    /************************************************************/

    void _drawBUTTON(UI_ELEMENT* pEl)
    {

    }

    void _drawBLANK(UI_ELEMENT* pEl)
    {
        SDL_SetRenderDrawColor(g_rend, pEl->frame->col.r, pEl->frame->col.g, pEl->frame->col.b, pEl->frame->col.a);
        const SDL_Rect out {pEl->frame->pos.x, pEl->frame->pos.y, pEl->frame->size.x, pEl->frame->size.y};
        SDL_RenderFillRect(g_rend, &out);

        lnode<UI_PICTURE*>* pIcn = pEl->pics.head;
        while (pIcn)
        {
            SDL_Rect o {pIcn->data->pos.x, pIcn->data->pos.y, pIcn->data->size.x, pIcn->data->size.y};
            SDL_RenderCopy(g_rend, pIcn->data->texture, nullptr, &o);
            pIcn = pIcn->nx;
        }

        lnode<UI_TEXT*>* pText = pEl->texts.head;
        while (pText)
        {
            SDL_Color col = {pText->data->col.r, pText->data->col.g, pText->data->col.b, pText->data->col.a};
            TTF_Font* fnt = res::res_getFont(0, 1);
            SDL_Surface* tmpText = TTF_RenderUTF8_Blended(fnt, pText->data->text, col);
            SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(g_rend, tmpText);
            SDL_Rect o {pText->data->pos.x, pText->data->pos.y};
            SDL_QueryTexture(tmpTexture, 0, 0, &o.w, &o.h);
            if (pText->data->alignment == 520551755) {o.x -= o.w / 2; o.y -= o.h / 2;} //center
            else if (pText->data->alignment == 601753) {o.x -= o.w;} //left
            SDL_RenderCopy(g_rend, tmpTexture, 0, &o);
            SDL_FreeSurface(tmpText);
            SDL_DestroyTexture(tmpTexture);
            pText = pText->nx;
        }
    }

    void _drawUI(UI_ELEMENT* pEl)
    {
        pDrawUI[pEl->frame->type](pEl);
    }

    void gut_UI()
    {
        static game::GAME_DATA* pd = game::getData();
        UI_CONTEXT UIContext = global::curUIContext();

        traverseTrie<UI_ELEMENT*>(pd->ui[UIContext]->root->childNodes.head, _drawUI);
        SDL_RenderPresent(g_rend);
    }

    /************************************************************/

    e_exitCodes gut_init() {

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            return GUT_SDL_ERR;

        if (TTF_Init() == -1)
            return GUT_TTF_ERR;

        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
            return GUT_IMG_ERR;

        g_wind = SDL_CreateWindow("CREN", 0, 0, getCFG().scrRenW, getCFG().scrRenH, SDL_WINDOW_SHOWN | (getCFG().isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) | SDL_WINDOW_OPENGL);
        SDL_DisplayMode md;
        SDL_GetCurrentDisplayMode(0, &md);
        getCFG().scrUIH = md.h; getCFG().scrUIW = md.w;
        if (g_wind == nullptr)
            return GUT_SDL_ERR;

        g_rend = SDL_CreateRenderer(g_wind, -1, SDL_RENDERER_ACCELERATED);
        if (g_rend == nullptr)
            return GUT_SDL_ERR;

        g_text = SDL_CreateTexture(g_rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, getCFG().scrRenW, getCFG().scrRenH);
        g_textBuf = new uint32_t[getCFG().scrRenW * getCFG().scrRenH];

        pDrawUI[0] = _drawBLANK;
        pDrawUI[1] = _drawBUTTON;

        return OK;
    }

    /************************************************************/

    void gut_clean() {
        SDL_DestroyRenderer(g_rend);
        SDL_DestroyWindow(g_wind);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
    }

    void gut_updWin(int w, int h, uint32_t flags) {
        getCFG().scrRenW = w;
        getCFG().scrRenH = h;

        delete g_textBuf;
        g_textBuf = new uint32_t[w * h];
        SDL_DestroyTexture(g_text);
        g_text = SDL_CreateTexture(g_rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, getCFG().scrRenW, getCFG().scrRenH);

        SDL_SetWindowFullscreen(g_wind, flags);
        SDL_SetWindowSize(g_wind, w, h);

        SDL_GetWindowSize(g_wind, &getCFG().scrUIW, &getCFG().scrUIH);
        std::cout << getCFG().scrUIW << " " << getCFG().scrUIH << std::endl;
        UI::setupUI();

    }

}
