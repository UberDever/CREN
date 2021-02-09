//
// Created by uberdever on 02.04.2020.
//

#ifndef CREN_UI_HPP
#define CREN_UI_HPP

#include "util.hpp"

//UI - user interface
namespace UI {

    //constexpr vars (UI)
    const constexpr char* uiPath = "../data/db/ui.xml";
    constexpr size_t tokenLen = 500;           //Length of token
    constexpr size_t depthLen = 10;           //Depth of token recursion
    constexpr size_t dataLen = 500;            //Length of text data
    constexpr size_t tokenNameLen = 100;
    constexpr size_t attrNameLen = 100;
    constexpr size_t valueLen = 100;
    constexpr size_t buttonKeyLen = 100;
    constexpr size_t hashTableSize = 101;

    //UI enums
    enum TYPES : size_t
    {
        STATIC    = 0x00,
        BUTTON    ,
        SLIDER    ,
        LIST      ,

        LAST     = 0x0F
    };

    enum CONTEXT : size_t
    {
        MAIN_SCREEN  = 0x00,
        GAME_PAUSE   ,
        OPTIONS      ,
        PLAYER_MENU  ,
        GAME_OVER    = 0x1F
    };

    //UI structs
    union DATA
    {
        int* pInt {nullptr};
        bool* pBool;
        float* pFloat;
    };

    struct BUTTON
    {
        enum BUTTON_TYPE : size_t {
            PUSH = 0x0,
            CHKBOX    ,
        };
        char key[buttonKeyLen]{};
        int val{0}; int def{0};
        bool isPressed {false};
        BUTTON_TYPE type {PUSH};
    };

    struct SLIDER
    {
        //TODO SLIDER
    };

    struct LIST
    {
        char key[buttonKeyLen]{};
        int val{0}; int def{0};
        int len{0};
        bool isPressed {false};
    };

    struct FRAME
    {
        math::v2<int> pos; math::v2<int> size;
        math::v2<int> relPos; math::v2<int> relSize;
        util::COLOR col {0}; bool isVisible {true};
    };

    struct TEXT
    {
        math::v2<int> pos; math::v2<int> relPos;
        char text[dataLen] {""}; int size {0};
        SDL_Texture* textTexture {nullptr};

        util::COLOR col {0}; TTF_Font *fnt {nullptr}; size_t alignment {520551755 /*center*/};
        bool isChanged {true}; bool isVisible {true};

        TEXT() = default;

        TEXT(TEXT& cp)
        {
            pos = cp.pos; relPos = cp.relPos;
            strcat(text, cp.text); size = cp.size;
            col = cp.col; fnt = cp.fnt; alignment = cp.alignment;
            isChanged = cp.isChanged; isVisible = cp.isVisible;
        }
    };

    struct PICTURE
    {
        math::v2<int> pos; math::v2<int> size;
        math::v2<int> relPos; math::v2<int> relSize;
        SDL_Texture* pic {nullptr}; bool isVisible {true};

        PICTURE() = default;

        PICTURE(PICTURE& cp)
        {
            pos = cp.pos; relPos = cp.relPos;
            size = cp.size; relSize = cp.relSize;
            pic = cp.pic; //Yes, that move is dangerous
            isVisible = cp.isVisible;
        }
        ~PICTURE() {SDL_DestroyTexture(pic);}
    };

    struct ELEMENT
    {
        FRAME* frame {nullptr};                      //frame is field of element, all events are processed in account of frame coordinates. It is also parent for all components of element
        util::list<TEXT*> texts;                 //all text elements in element
        util::list<PICTURE*> pics;               //all pictures and icons
        TYPES type {STATIC};
        int id {-1};

        union {
            struct BUTTON* btn;
            struct SLIDER* sld;
            struct LIST* lst {nullptr};
        };

        util::list<ELEMENT*> childNodes;   //all child nodes for this element

        ~ELEMENT() {
            switch (type) {
                case BUTTON: delete btn; break;
                case SLIDER: delete sld; break;
                case LIST: delete lst; break;
                default: break;
            }
            delete frame;
            texts.clean_heap();
            pics.clean_heap();
            childNodes.clean_heap();
        }
    };

    struct SCENE
    {
        ELEMENT* root {new ELEMENT()}; //first element is root element
        CONTEXT id {MAIN_SCREEN};

        SCENE() = default;
        ~SCENE() {delete root;}
    };

    util::list<SCENE*>* getUI();
    util::hashT<DATA>* getUI_VARS();

    void setupChildUI(ELEMENT* pEl);

    util::e_exitCodes UI_init();
    void UI_clean();
}

#endif //CREN_UI_HPP
