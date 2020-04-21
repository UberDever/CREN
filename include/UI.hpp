//
// Created by uberdever on 02.04.2020.
//

#ifndef CREN_UI_HPP
#define CREN_UI_HPP

#include "util.hpp"

//UI - user interface
namespace UI {
    using namespace util;
    using namespace math;

    //constexpr vars (UI)
    constexpr int UITextLen = 200;

    //UI enums
    enum UI_TYPES : uint32_t
    {
        BLANK    = 10577140,
        BUTTON   = 415276622,
        SLIDER   = 520070527,

        LAST     = 0x0F
    };

    enum UI_CONTEXT : uint8_t
    {
        MAIN_SCREEN  = 0x00,
        GAME_PAUSE   = 0x01,
        OPTIONS      = 0x02,
        INV          = 0x03,
        GAME_OVER    = 0x04
    };

    //UI structs
    struct UI_FRAME
    {
        math::v2<int> pos; math::v2<int> size;
        math::v2<int> relPos; math::v2<int> relSize;
        COLOR col;
        UI_TYPES type;

        UI_FRAME() : col{0}, type{BLANK} {}
    };

    struct UI_TEXT
    {
        math::v2<int> pos; math::v2<int> relPos;
        char text[UITextLen]; uint32_t size;

        COLOR col; uint32_t fontType; uint32_t alignment;

        UI_TEXT () : text{""}, col{0}, fontType{0}, size{0}, alignment(520551755 /*center*/) {}
    };

    struct UI_PICTURE
    {
        math::v2<int> pos; math::v2<int> size;
        math::v2<int> relPos; math::v2<int> relSize;
        SDL_Texture* texture;

        UI_PICTURE () : texture{nullptr} {}
        ~UI_PICTURE() {SDL_DestroyTexture(texture);}
    };

    struct UI_ELEMENT
    {
        UI_FRAME* frame;                      //frame is field of element, all events are processed in account of frame coordinates. It is also parent for all components of element
        list<UI_TEXT*> texts;                 //all text elements in element
        list<UI_PICTURE*> pics;               //all pictures and icons
        uint32_t id;

        list<UI_ELEMENT*> childNodes;   //all child nodes for this element

        UI_ELEMENT() : frame{nullptr}, id{0} {}
        ~UI_ELEMENT() {delete frame; texts.clean(); pics.clean(); childNodes.clean();}
    };

    struct UI_SCENE
    {
        UI_ELEMENT* root; //first element is root element
        UI_CONTEXT id;

        UI_SCENE() : id{MAIN_SCREEN}, root{new UI_ELEMENT()} {root->id = -1;}
        ~UI_SCENE() {delete root;}
    };

    list<UI_SCENE*>* getUI();
    void setupUI(); //Used for changing UI according to resolution

    e_exitCodes UI_init();
    void UI_clean();
}

#endif //CREN_UI_HPP
