//
// Created by uberdever on 02.04.2020.
//

#ifndef CREN_UI_HPP
#define CREN_UI_HPP

#include "util.hpp"

//UI - user interface
namespace UI {

    //constexpr vals
    constexpr int UITextLen = 200;

    using namespace util;
    using namespace math;

    enum UI_TYPES : uint8_t
    {
        BLANK    = 0x00,
        BUTTON   = 0x01,
        SLIDER   = 0x02,

        LAST     = 0xF0
    };

    enum UI_CONTEXT : uint8_t
    {
        MAIN_SCREEN  = 0x00,
        OPTIONS      = 0x01,
        INV          = 0x02,
        GAME_OVER    = 0x03
    };

    struct UI_FRAME
    {
        v2<int> pos; v2<int> size;
        uint32_t color;
        UI_TYPES type;

        UI_FRAME() : pos{0, 0}, size{0, 0}, color{0}, type{BLANK} {}
        UI_FRAME(v2<int>_pos, v2<int>_size, uint32_t _color, UI_TYPES _type) :
        pos{_pos}, size{_size}, color{_color}, type{_type} {}
    };

    struct UI_TEXT
    {
        v2<int> pos; char text[UITextLen]; uint32_t size;

        uint32_t color; uint32_t fontType;

        UI_TEXT () : pos{0, 0}, text{""}, color{0}, fontType{0}, size{0} {}
    };

    struct UI_PICTURE
    {
        v2<int> pos; v2<int> size;
        SDL_Surface* surf;

        UI_PICTURE () :pos{0, 0}, size{0, 0}, surf{nullptr} {}
    };

    struct UI_ELEMENT
    {
        UI_FRAME* frame;                      //frame is field of element, all events are processed in account of frame coordinates. It is also parent for all components of element
        list<UI_TEXT*> texts;                 //all text elements in element
        list<UI_PICTURE*> pics;               //all pictures and icons
        uint32_t id;

        list<UI_ELEMENT*> childNodes;   //all child nodes for this element

        UI_ELEMENT() : frame{nullptr}, id{0} {}
    };

    struct UI_SCENE
    {
        UI_ELEMENT* root; //first element is root element
        uint32_t id;

        UI_SCENE() : id{0}, root{new UI_ELEMENT()} {root->id = -1;}
    };

    //UI_SCENE* getContext();

    e_exitCodes UI_init();

    void UI_clean();
}

#endif //CREN_UI_HPP
