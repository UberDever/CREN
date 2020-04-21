//
// Created by uberdever on 02.04.2020.
//

#include "UI.hpp"

//for populating all of UI windows
namespace UI
{
    list<UI_SCENE*>* ui;

    list<UI_SCENE*>* getUI() {
        return ui;
    }

    v2<int> parseV2(const char* str)
    {
        char* p;
        int x = strtol(str, &p, 10);
        int y = strtol(p, nullptr, 10);
        return v2<int>{x, y};
    }

    e_exitCodes parseUI() //It is real fully-functional xml parser!
    {
        constexpr char* confPath = "../data/cfg/ui.xml";
        constexpr uint32_t tokenLen = 100;           //Length of token
        constexpr uint32_t depthLen = 10;           //Depth of token recursion
        constexpr uint32_t dataLen = UITextLen;            //Length of data
        constexpr uint32_t tokenNameLen = 25;
        constexpr uint32_t attrNameLen = 25;
        constexpr uint32_t valueLen = 25;

        UI_SCENE* * curScene = nullptr; //pointer to scene (which is also pointer)
        UI_ELEMENT* * curElement[depthLen] = {nullptr}; //very complicated construction, it's just pointer to element (which is also pointer), with various depth degrees

        FILE* in; in = fopen(confPath, "r"); if (!in) return UI_CONF_ERR;
        char token[tokenLen] = {};
        char data[dataLen] = {};
        uint32_t dataIndex = 0;
        uint32_t depth = 0;
        while (!feof(in))
        {
            while (fscanf(in, "<%[^>]s", token) == 0)
            {
                data[dataIndex] = (char)getc(in);
                if (data[dataIndex] == '#') {fscanf(in, "%*[^\n]\n");}
                dataIndex = (dataIndex + 1) % dataLen;
            }
            data[dataIndex] = 0;
            if (token[0] == '/')
            {
                switch (hashStr(token))
                {
                    case 19190780: {strcat((*curElement[depth])->texts.back()->text, data + 1); break;} //text
                    case 832640817: {if (depth > 0) depth--; break;}
                    case 9251: break; //ui
                    case 156227427: break; //scene
                    default: return UI_PARSE_ERR;
                }
                dataIndex = 0;
                continue;
            }
            else
            {
                uint32_t scanned = 0;
                char name[tokenNameLen] = {};
                char attr[attrNameLen] = {};
                char value[valueLen] = {};
                sscanf(token, "%s %n", name, &scanned);
                memcpy(token, token + scanned, strlen(token));
                switch (hashStr(name))
                {
                    case 5039596: //scene
                    {
                        ui->append(new UI_SCENE());
                        curScene = &ui->back();
                        curElement[0] = &(*curScene)->root;
                        break;
                    }
                    case 155891642: //element
                    {
                        (*curElement[depth])->childNodes.append(new UI_ELEMENT());
                        curElement[depth + 1] = &(*curElement[depth])->childNodes.back();
                        depth++;
                        break;
                    }
                    case 5006413: {(*curElement[depth])->frame = new UI_FRAME(); break;} //frame
                    case 619059: {(*curElement[depth])->texts.append(new UI_TEXT()); break;} //text
                    case 972836026: {(*curElement[depth])->pics.append(new UI_PICTURE()); break;} //picture
                    case 300: break; //ui
                    default: return UI_PARSE_ERR;
                }
                while (sscanf(token, "%[^=]s", attr) != -1)
                {
                    sscanf(token, R"(%*[^"]"%n%[^"]s)", &scanned, value); //Raw string literal, from c++11
                    memcpy(token, token + scanned + strlen(value) + 2, strlen(token));
                    //std::cout << "name: " << name << " attr: " << attr << " value: " << value << " hash: " << hashStr("left") << " " << (int)(SDL_WINDOW_MAXIMIZED) << std::endl;
                    switch (hashStr(name))
                    {
                        case 5039596: { if (hashStr(attr) == 133) (*curScene)->id = static_cast<UI_CONTEXT>(strtol(value, nullptr, 10)); break;} //scene
                        case 155891642: { if (hashStr(attr) == 133) (*curElement[depth])->id = strtol(value, nullptr, 10); break;} //element
                        case 5006413: //frame
                        {
                            switch (hashStr(attr))
                            {
                                case 18740: { (*curElement[depth])->frame->relPos = parseV2(value); break;} //pos
                                case 174239: { (*curElement[depth])->frame->relSize = parseV2(value);break;} //size
                                case 17082243:
                                {
                                    (*curElement[depth])->frame->col.color = strtol(value, nullptr, 16);
                                    break;
                                } //color
                                case 165126: { (*curElement[depth])->frame->type = static_cast<UI_TYPES>(strtol(value, nullptr, 10)); break;} //type
                                default: return UI_PARSE_ERR;
                            }
                            break;
                        }
                        case 619059: //text
                        {
                            switch (hashStr(attr))
                            {
                                case 18740: { (*curElement[depth])->texts.back()->relPos = parseV2(value); break;} //pos
                                case 174239: { (*curElement[depth])->texts.back()->size = strtol(value, nullptr, 10); break;} //size
                                case 17082243: { (*curElement[depth])->texts.back()->col.color = strtol(value, nullptr, 16); break;} //color
                                case 609745:
                                {
                                    char path[100] = "../data/fonts/";
                                    (*curElement[depth])->texts.back()->fontType = res::res_getFontType(value);
                                    if ((*curElement[depth])->texts.back()->fontType == (uint32_t)-1)
                                        return UI_PARSE_ERR;
                                    break;
                                } //font
                                case 13146853: {(*curElement[depth])->texts.back()->alignment = hashStr(value); break; } //align 601753 520551755 18715772 left center right}
                                default: return UI_PARSE_ERR;
                            }
                            break;
                        }
                        case 972836026: //picture
                        {
                            switch (hashStr(attr))
                            {
                                case 18740: { (*curElement[depth])->pics.back()->relPos = parseV2(value); break;} //pos
                                case 174239: { (*curElement[depth])->pics.back()->relSize = parseV2(value); break;} //size
                                case 7177:
                                {
                                    char path[100] = "../data/sprites/UI/";
                                    SDL_Surface* tmp = res::res_loadPNG(strcat(path, value));
                                    (*curElement[depth])->pics.back()->texture = SDL_CreateTextureFromSurface(gut::getR(),tmp);
                                    SDL_FreeSurface(tmp);
                                    if ((*curElement[depth])->pics.back()->texture == nullptr) return UI_PARSE_ERR;
                                    break;
                                } //png
                                default: return UI_PARSE_ERR;
                            }
                            break;
                        }
                        default: break;
                    }
                }
            }
            dataIndex = 0;
        }
        return OK;
    }

    void setupChildUI(UI_ELEMENT* pEl)
    {
        lnode<UI_ELEMENT*>* pList = pEl->childNodes.head;
        while (pList)
        {
            pList->data->frame->pos = pEl->frame->pos +
                    v2<int>{(pList->data->frame->relPos.x * pEl->frame->size.x) / 100, (pList->data->frame->relPos.y * pEl->frame->size.y) / 100};
            pList->data->frame->size = v2<int>{(pList->data->frame->relSize.x * pEl->frame->size.x) / 100, (pList->data->frame->relSize.y * pEl->frame->size.y) / 100};

            lnode<UI_TEXT*>* pText = pList->data->texts.head;
            while (pText)
            {
                pText->data->pos = pList->data->frame->pos +
                        v2<int>{(pText->data->relPos.x * pList->data->frame->size.x) / 100, (pText->data->relPos.y * pList->data->frame->size.y) / 100};
                pText = pText->nx;
            }

            lnode<UI_PICTURE*>* pIcn = pList->data->pics.head;
            while (pIcn)
            {
                pIcn->data->size = v2<int>{pList->data->frame->size.x * pIcn->data->relSize.x / 100, pList->data->frame->size.y * pIcn->data->relSize.y / 100};
                pIcn->data->pos = pList->data->frame->pos +
                                   v2<int>{(pIcn->data->relPos.x * pList->data->frame->size.x) / 100, (pIcn->data->relPos.y * pList->data->frame->size.y) / 100};
                pIcn = pIcn->nx;
            }

            pList = pList->nx;
        }
    }

    void setupUI()
    {
        lnode<UI_SCENE*>* pUI = ui->head;
        while (pUI)
        {
            lnode<UI_ELEMENT*>* pList = pUI->data->root->childNodes.head;
            while (pList)
            {
                pList->data->frame->pos = v2<int>{(pList->data->frame->relPos.x * cfg::getCFG().scrUIW) / 100, (pList->data->frame->relPos.y * cfg::getCFG().scrUIH) / 100};
                pList->data->frame->size = v2<int>{(pList->data->frame->relSize.x * cfg::getCFG().scrUIW) / 100, (pList->data->frame->relSize.y * cfg::getCFG().scrUIH) / 100};

                lnode<UI_TEXT*>* pText = pList->data->texts.head;
                while (pText)
                {
                    pText->data->pos = pList->data->frame->pos +
                                       v2<int>{(pText->data->relPos.x * pList->data->frame->size.x) / 100, (pText->data->relPos.y * pList->data->frame->size.y) / 100};
                    pText = pText->nx;
                }

                lnode<UI_PICTURE*>* pIcn = pList->data->pics.head;
                while (pIcn)
                {
                    pIcn->data->size = v2<int>{pList->data->frame->size.x * pIcn->data->relSize.x / 100, pList->data->frame->size.y * pIcn->data->relSize.y / 100};
                    pIcn->data->pos = pList->data->frame->pos +
                                      v2<int>{(pIcn->data->relPos.x * pList->data->frame->size.x) / 100, (pIcn->data->relPos.y * pList->data->frame->size.y) / 100};
                    pIcn = pIcn->nx;
                }
                pList = pList->nx;
            }
            traverseTrie<UI_ELEMENT*>(pUI->data->root->childNodes.head, setupChildUI);
            pUI = pUI->nx;
        }
    }

    e_exitCodes UI_init()
    {
        ui = new list<UI_SCENE*>();
        e_exitCodes exitCode = parseUI();
        if (exitCode != OK) return exitCode;
        setupUI();
        return exitCode;
    }

    void UI_clean() {
        ui->clean();
        //TODO CLEAN UI
    }

}