//
// Created by uberdever on 02.04.2020.
//

#include "UI.hpp"

namespace UI
{
    util::list<SCENE*>* ui;
    util::hashT<DATA>* ui_vars; //table of variables, changable by ui

    util::list<SCENE*>* getUI() {
        return ui;
    }

    util::hashT<DATA> *getUI_VARS() {
        return ui_vars;
    }

    util::e_exitCodes parseUI() //It is real fully-functional xml parser!
    {
        SCENE* * curScene = nullptr; //pointer to scene (which is also pointer)
        ELEMENT* * curElement[depthLen] = {nullptr}; //very complicated construction, it's just pointer to element (which is also pointer), with various depth degrees

        FILE* in; in = fopen(uiPath, "r"); if (!in) return util::UI_PARSE_ERR;
        char token[tokenLen] = {};
        char data[dataLen] = {};
        size_t dataIndex = 0;
        size_t depth = 0;
        size_t idCounter = 0;
        while (true)
        {
            while (fscanf(in, "<%[^>]s", token) == 0)
            {
                data[dataIndex] = (char)getc(in);
                if (data[dataIndex] == '#') {fscanf(in, "%*[^\n]\n");}
                dataIndex = (dataIndex + 1) % dataLen;
            }
            if (feof(in)) break;
            data[dataIndex] = 0;
            if (token[0] == '/')
            {
                switch (util::hash(token, strlen(token)))
                {
                    case 2112856514: {strcat((*(*curElement[depth])->texts.back())->text, data + 1);  break;} // /text
                    case 3178124386: {idCounter = 0; if (depth > 0) depth--; break;} // /element
                    case 2502698063:
                    {
                        if ((*curElement[depth])->btn->type == BUTTON::CHKBOX)
                        {
                            (*curElement[depth])->btn->isPressed = (*curElement[depth])->btn->def;
                            (*(*curElement[depth])->texts.back())->isVisible = (*curElement[depth])->btn->def;
                        }
                        break;
                    } //button
                    case 2420293948:
                    {
                        (*curElement[depth])->lst->val = (*curElement[depth])->lst->def;
                        break;
                    } //list
                    case 2019111202: break; // /ui
                    case 3259774813: break; // /scene
                    default: std::cout << "/token\n"; return util::UI_PARSE_ERR;
                }
                dataIndex = 0;
                continue;
            }
            else
            {
                uint32_t scanned = 0;
                char value[valueLen] = {};
                char name[tokenNameLen] = {};
                char attr[attrNameLen] = {};

                sscanf(token, "%s %n", name, &scanned);
                memcpy(token, token + scanned, strlen(token));

                uint32_t hashName = util::hash(name, strlen(name));
                switch (hashName)
                {
                    case 1928838231: //scene
                    {
                        ui->append(new SCENE());
                        curScene = ui->back();
                        curElement[0] = &(*curScene)->root;
                        break;
                    }
                    case 823327721: //element
                    {
                        (*curElement[depth])->childNodes.append(new ELEMENT());
                        (*curElement[depth])->id = idCounter;
                        idCounter++;
                        curElement[depth + 1] = (*curElement[depth])->childNodes.back();
                        depth++;
                        break;
                    }
                    case 3698242818: {(*curElement[depth])->frame = new FRAME(); break;} //frame
                    case 3409410179: {(*curElement[depth])->texts.append(new TEXT()); break;} //text
                    case 903569893: {(*curElement[depth])->pics.append(new PICTURE()); break;} //picture
                    case 3764576718: {(*curElement[depth])->btn = new struct BUTTON(); break;} //button
                    case 1827021842: {(*curElement[depth])->lst = new struct LIST(); break;} //list
                    case 4023624386: break; //ui
                    default: std::cout << "token name\n"; return util::UI_PARSE_ERR;
                }
                while (sscanf(token, "%[^=]s", attr) != -1)
                {
                    value[0] = 0;
                    sscanf(token, R"(%*[^"]"%n%[^"]s)", &scanned, value); //Raw string literal, from c++11
                    memcpy(token, token + scanned + strlen(value) + 2, strlen(token));
                    uint32_t hashAttr = util::hash(attr, strlen(attr));
                    //char* str = "grid_size";
                    //std::cout << "name: " << name << " attr: " << attr << " value: " << value << " hash: " << hash(str, strlen(str)) << std::endl;
                    switch (hashName)
                    {
                        case 1928838231: { if (util::hash(attr, strlen(attr)) == 93303930 /*id*/) (*curScene)->id = static_cast<CONTEXT>(atoi(value)); break;} //scene
                        case 823327721:
                        {
                            if (hashAttr == 93303930) {(*curElement[depth])->id = atoi(value);} //id
                            else {std::cout << "element\n"; return util::UI_PARSE_ERR;}
                            break;
                        } //element
                        case 3698242818: //frame
                        {
                            switch (hashAttr)
                            {
                                case 1521871997: { (*curElement[depth])->frame->relPos = util::parseV2(value); break;} //pos
                                case 1255330124: { (*curElement[depth])->frame->relSize = util::parseV2(value); break;} //size
                                case 4269486905:
                                {
                                    (*curElement[depth])->frame->col.color = strtol(value, nullptr, 16);
                                    break;
                                } //color
                                case 901001423: {(*curElement[depth])->frame->isVisible = (value[0] != 'f'); break;} //visible
                                default: std::cout << name << " " << attr << " frame\n"; return util::UI_PARSE_ERR;
                            }
                            break;
                        }
                        case 3409410179: //text
                        {
                            switch (hashAttr)
                            {
                                case 1521871997: { (*(*curElement[depth])->texts.back())->relPos = util::parseV2(value); break;} //pos
                                case 1255330124: { (*(*curElement[depth])->texts.back())->size = atoi(value); break;} //size
                                case 4269486905: { (*(*curElement[depth])->texts.back())->col.color = strtol(value, nullptr, 16); break;} //color
                                case 1399703508: {
                                    (*(*curElement[depth])->texts.back())->fnt = res::res_getFont(value, (*(*curElement[depth])->texts.back())->size);
                                    if (!(*(*curElement[depth])->texts.back())->fnt)
                                        return util::UI_PARSE_ERR;
                                    break;
                                } //font
                                case 1196188182: {(*(*curElement[depth])->texts.back())->alignment = util::hash(value, strlen(value)); break; } //align 3032534746 3215930507 1861870502 (left center right} 1196188182
                                case 901001423: {(*(*curElement[depth])->texts.back())->isVisible = (value[0] != 'f'); break;} //visible
                                default: std::cout << "text\n"; return util::UI_PARSE_ERR;
                            }
                            break;
                        }
                        case 903569893: //picture
                        {
                            switch (hashAttr)
                            {
                                case 1521871997: { (*(*curElement[depth])->pics.back())->relPos = util::parseV2(value); break;} //pos
                                case 1255330124: { (*(*curElement[depth])->pics.back())->relSize = util::parseV2(value); break;} //size
                                case 3708448090: { (*(*curElement[depth])->pics.back())->size = util::parseV2(value); break;} //grid_size
                                case 2279514204: //grid
                                {
                                    math::v2<int> size = (*(*curElement[depth])->pics.back())->size;
                                    char path[100] = "../data/sprites/UI/";
                                    SDL_Surface * tmp = res::res_loadPNG(strcat(path, value));
                                    SDL_Surface * out = SDL_CreateRGBSurface(0, size.x * tmp->w,
                                                                             size.y * tmp->h,
                                                                             32, 0, 0, 0, 0);
                                    for (int x = 0; x < size.x; x++)
                                    {
                                        for (int y = 0; y < size.y; y++)
                                        {
                                            SDL_Rect o {x * tmp->w, y * tmp->h, tmp->w, tmp->h};
                                            SDL_BlitSurface(tmp, 0, out, &o);
                                        }
                                    }
                                    (*(*curElement[depth])->pics.back())->pic = SDL_CreateTextureFromSurface(global::renderer(), out);
                                    SDL_FreeSurface(out);
                                    SDL_FreeSurface(tmp);
                                    if (!(*(*curElement[depth])->pics.back())->pic) return util::UI_PARSE_ERR;
                                    break;
                                }
                                case 2665484906:
                                {
                                    char path[100] = "../data/sprites/UI/";
                                    SDL_Surface * tmp = res::res_loadPNG(strcat(path, value));
                                    (*(*curElement[depth])->pics.back())->pic = SDL_CreateTextureFromSurface(global::renderer(), tmp);
                                    SDL_FreeSurface(tmp);
                                    if (!(*(*curElement[depth])->pics.back())->pic) return util::UI_PARSE_ERR;
                                    break;
                                } //png
                                case 901001423: {(*(*curElement[depth])->pics.back())->isVisible = (value[0] != 'f'); break;} //visible
                                default: std::cout << "picture\n"; return util::UI_PARSE_ERR;
                            }
                            break;
                        }
                        case 3764576718:  //button
                        {
                            (*curElement[depth])->type = BUTTON;
                            switch (hashAttr)
                            {
                                case 3331274845: //var
                                {
                                    strncpy((*curElement[depth])->btn->key, value, buttonKeyLen);
                                    break;
                                }
                                case 3788914869: //value
                                {
                                    (*curElement[depth])->btn->val = atoi(value);
                                    break;
                                }
                                case 428786659: //default
                                {
                                    (*curElement[depth])->btn->def = atoi(value);
                                    break;
                                }
                                case 1780700712:
                                {
                                    (*curElement[depth])->btn->type = BUTTON::CHKBOX;
                                    break;
                                } //checkbox
                                default: std::cout << "button\n"; return util::UI_PARSE_ERR;
                            }
                            break;
                        }
                        case 1827021842: //list 1827021842
                        {
                            (*curElement[depth])->type = LIST;
                            switch (hashAttr)
                            {
                                case 3331274845: //var
                                {
                                    strncpy((*curElement[depth])->lst->key, value, buttonKeyLen);
                                    break;
                                }
                                case 3788914869: //value
                                {
                                    char* it = value; size_t counter = 1; int k = 1; int index = 0;
                                    char tmpStr[valueLen] {0};
                                    while ((*it) != '\0') {if (*it == ' ') counter++; it++;}
                                    (*curElement[depth])->lst->len = counter;
                                    util::lNode<TEXT*>* pText = (*curElement[depth])->texts.head;
                                    util::lNode<PICTURE*>* pIcn = (*curElement[depth])->pics.head;
                                    while (k)
                                    {
                                        k = 0;
                                        sscanf(value, "%s%n", tmpStr, &k);

                                        if (k)
                                        {
                                            memcpy(value, value + k, strlen(value));

                                            (*curElement[depth])->texts.append(new TEXT(*pText->data));
                                            strcat((*(*curElement[depth])->texts.back())->text, tmpStr);
                                            (*(*curElement[depth])->texts.back())->relPos.y = (pText->data->relPos.y / counter) + ((index * 100) / counter);
                                            (*(*curElement[depth])->texts.back())->isVisible = false;

                                            (*curElement[depth])->pics.append(new PICTURE(*pIcn->data));
                                            (*(*curElement[depth])->pics.back())->relSize.y = (100 / counter);
                                            (*(*curElement[depth])->pics.back())->relPos.y = ((index * 100) / counter);
                                            (*(*curElement[depth])->pics.back())->isVisible = false;

                                            tmpStr[0] = 0;
                                        }
                                        index++;
                                    }
                                    //strcat(pText->data->text, pText->nx->data->text);
                                    break;
                                }
                                case 428786659: //default
                                {
                                    auto* it = (*curElement[depth])->texts.head; int index = 0;
                                    for (; strcmp(it->data->text, value); it = it->nx, index++);
                                    (*curElement[depth])->texts.head->data->text[0] = 0;
                                    strcat((*curElement[depth])->texts.head->data->text, it->data->text);
                                    (*curElement[depth])->lst->def = index - 1;
                                    break;
                                }
                                default: std::cout << "list\n"; return util::UI_PARSE_ERR;
                            }
                            break;
                        }
                        default: std::cout << "token name\n"; return util::UI_PARSE_ERR;
                    }
                }
            }
            dataIndex = 0;
        }
        return util::OK;
    }

    void setupChildUI(ELEMENT* pEl)
    {
        util::lNode<ELEMENT*>* pList = pEl->childNodes.head;
        while (pList)
        {
            pList->data->frame->pos = pEl->frame->pos +
                    math::v2<int>{(pList->data->frame->relPos.x * pEl->frame->size.x) / 100, (pList->data->frame->relPos.y * pEl->frame->size.y) / 100};
            pList->data->frame->size = math::v2<int>{(pList->data->frame->relSize.x * pEl->frame->size.x) / 100, (pList->data->frame->relSize.y * pEl->frame->size.y) / 100};

            util::lNode<TEXT*>* pText = pList->data->texts.head;
            while (pText)
            {
                pText->data->pos = pList->data->frame->pos +
                        math::v2<int>{(pText->data->relPos.x * pList->data->frame->size.x) / 100, (pText->data->relPos.y * pList->data->frame->size.y) / 100};
                pText = pText->nx;
            }

            util::lNode<PICTURE*>* pIcn = pList->data->pics.head;
            while (pIcn)
            {
                pIcn->data->size = math::v2<int>{pList->data->frame->size.x * pIcn->data->relSize.x / 100, pList->data->frame->size.y * pIcn->data->relSize.y / 100};
                pIcn->data->pos = pList->data->frame->pos +
                        math::v2<int>{(pIcn->data->relPos.x * pList->data->frame->size.x) / 100, (pIcn->data->relPos.y * pList->data->frame->size.y) / 100};
                pIcn = pIcn->nx;
            }

            pList = pList->nx;
        }
    }

    void setupUI()
    {
        util::lNode<SCENE*>* pUI = ui->head;
        while (pUI)
        {
            util::lNode<ELEMENT*>* pList = pUI->data->root->childNodes.head;
            while (pList)
            {
                pList->data->frame->pos = math::v2<int>{(pList->data->frame->relPos.x * global::cfg::scrUIW()) / 100, (pList->data->frame->relPos.y * global::cfg::scrUIH()) / 100};
                pList->data->frame->size = math::v2<int>{(pList->data->frame->relSize.x * global::cfg::scrUIW()) / 100, (pList->data->frame->relSize.y * global::cfg::scrUIH()) / 100};

                util::lNode<TEXT*>* pText = pList->data->texts.head;
                while (pText)
                {
                    pText->data->pos = pList->data->frame->pos +
                            math::v2<int>{(pText->data->relPos.x * pList->data->frame->size.x) / 100, (pText->data->relPos.y * pList->data->frame->size.y) / 100};
                    pText = pText->nx;
                }

                util::lNode<PICTURE*>* pIcn = pList->data->pics.head;
                while (pIcn)
                {
                    pIcn->data->size = math::v2<int>{pList->data->frame->size.x * pIcn->data->relSize.x / 100, pList->data->frame->size.y * pIcn->data->relSize.y / 100};
                    pIcn->data->pos = pList->data->frame->pos +
                            math::v2<int>{(pIcn->data->relPos.x * pList->data->frame->size.x) / 100, (pIcn->data->relPos.y * pList->data->frame->size.y) / 100};
                    pIcn = pIcn->nx;
                }
                pList = pList->nx;
            }
            util::traverseTrie<ELEMENT*>(pUI->data->root->childNodes.head, setupChildUI);
            pUI = pUI->nx;
        }
    }

    util::e_exitCodes UI_init()
    {
        ui = new util::list<SCENE*>();
        ui_vars = new util::hashT<DATA>(hashTableSize);
        util::e_exitCodes exitCode = parseUI();
        if (exitCode != util::OK) return exitCode;
        setupUI();
        return exitCode;
    }

    void UI_clean() {
        ui->clean_heap();
        delete ui;
        //TODO CLEAN UI
    }

}