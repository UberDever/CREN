//
// Created by uberdever on 02.04.2020.
//

#include "UI.hpp"

//for populating all of UI windows
namespace UI
{
    list<UI_SCENE*>* ui;
    UI_ELEMENT* uiElements;

    //lnode<UI_SCENE*> getContext() {return ui.head;}

    e_exitCodes parseUI()
    {
        constexpr char* confPath = "../data/cfg/ui.conf";
        constexpr uint32_t tokenLen = 50;           //Length of token
        constexpr uint32_t depthLen = 10;           //Depth of token recursion
        constexpr uint32_t dataLen = 20;            //Length of data

        UI_SCENE* curScene = nullptr;
        UI_ELEMENT* curElement[depthLen] = {nullptr};

        FILE* in; in = fopen(confPath, "r"); if (!in) return UI_CONF_ERR;
        char token[depthLen][tokenLen] = {};
        char data[dataLen] = {};
        uint32_t tokenIndex = 0;
        uint32_t dataIndex = 0;
        uint32_t depth = 0;
        while (!feof(in))
        {
            while (fscanf(in, "<%[^>]s>", token[tokenIndex]) == 0)
            {
                data[dataIndex] = (char)getc(in);
                dataIndex = (dataIndex + 1) % dataLen;
            }
            data[dataIndex] = 0;

            if (token[tokenIndex][0] == '/')
            {
                memcpy(data, data + 1, dataIndex);
                //printf("%s\n", data);
                //for (int i = 0; i <= tokenIndex; ++i) {
                //    printf("%s->", token[i]); //TODO REMOVE
                //}
                //printf("\b\b = %s\n", data); //TODO REMOVE*/
                if (!strcmp(token[tokenIndex], "/scene"))
                {
                    std::cout << "/1" << " ";
                }
                else if (!strcmp(token[tokenIndex], "/element"))
                {
                    depth--;
                    std::cout << "/" << 2 << " ";
                }
                else if (!strcmp(token[tokenIndex], "/pos"))
                {
                    char* p;
                    int x = strtol(data, &p, 10);
                    int y = strtol(p, nullptr, 10);
                    if (curElement[depth]->frame != nullptr) {curElement[depth]->frame->pos = v2<int>{x, y};}
                    else if (curElement[depth]->texts != nullptr) {curElement[depth]->texts->pos = v2<int>{x, y};}
                    else {curElement[depth]->pics->pos = v2<int>{x, y};}
                }
                else if (!strcmp(token[tokenIndex], "/size"))
                {
                    char* p;
                    int x = strtol(data, &p, 10);
                    int y = strtol(p, nullptr, 10);
                    if (curElement[depth]->frame != nullptr) {curElement[depth]->frame->size = v2<int>{x, y};}
                    else if (curElement[depth]->texts != nullptr) {curElement[depth]->texts->size = x;}
                    else {curElement[depth]->pics->size = v2<int>{x, y};}
                }
                else if (!strcmp(token[tokenIndex], "/color"))
                {
                    std::cout << "/4" << " ";
                }
                else if (!strcmp(token[tokenIndex], "/type"))
                {
                    std::cout << "/5" << " ";
                }
                tokenIndex--;
                dataIndex = 0;
                continue;
            }
            else
            {
                if (!strcmp(token[tokenIndex], "scene"))
                {
                    ui->append(new UI_SCENE());
                    curScene = ui->back();
                    curElement[depth] = curScene->root;

                    std::cout << 1 << " ";
                }
                else if (!strcmp(token[tokenIndex], "element"))
                {
                    //std::cout << depth << " ";
                    curElement[depth]->childNodes.append(new UI_ELEMENT());
                    curElement[depth + 1] = curElement[depth]->childNodes.back();
                    depth++;
                }
                else if (!strcmp(token[tokenIndex], "frame"))
                {
                    curElement[depth]->frame = new UI_FRAME();
                    std::cout << 3 << " ";
                }
                else if (!strcmp(token[tokenIndex], "text"))
                {
                    curElement[depth]->texts = new UI_TEXT();
                    std::cout << 4 << " ";
                }
                else if (!strcmp(token[tokenIndex], "picture"))
                {
                    curElement[depth]->pics = new UI_PICTURE();
                    std::cout << 5 << " ";
                }
            }
            tokenIndex++;
            dataIndex = 0;
            //printf("%s\n", data); TODO REMOVE

        }
        return OK;
    }

    e_exitCodes UI_init()
    {
        uiElements = new UI_ELEMENT[UI_TYPES::LAST];
        ui = new list<UI_SCENE*>();

        //Blank frame
        //uiElements[UI_TYPES::BLANK].frame = new UI_FRAME();
        //uiElements[UI_TYPES::BLANK].frame->type = UI_TYPES::BLANK;
        //uiElements[UI_TYPES::BLANK].frame->color = 0x0;

        //Button
        //uiElements[UI_TYPES::BUTTON].frame = new UI_FRAME();
        //uiElements[UI_TYPES::BUTTON].pics = new UI_PICTURE();
        //uiElements[UI_TYPES::BUTTON].texts = new UI_TEXT();

        e_exitCodes exitCode = parseUI();
        exitCode = UI_CONF_ERR;
        return exitCode;
    }

    void UI_clean() {
        //TODO CLEAN UI
    }
}