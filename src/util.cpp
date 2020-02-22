//
// Created by uberdever on 19.02.2020.
//

#include "util.hpp"

namespace util
{
    c_gameEvent parse_event(SDL_Event* event) {
        uint32_t eventCodes[SDL_ParseCount] = {SDL_QUIT, SDL_WINDOWEVENT, SDL_KEYDOWN, SDL_KEYUP, SDL_MOUSEMOTION,
                                  SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP, SDL_MOUSEWHEEL};
        int index = 0;
        for (; index < SDL_ParseCount && (event->type ^ eventCodes[index]); index++);
        c_gameEvent parsedEvent;
        parsedEvent.mx = event->motion.x;
        parsedEvent.my = event->motion.y;
        parsedEvent.mbutton = event->button.button;
        parsedEvent.type = index;
        return parsedEvent;
        //std::cout << index << " " << parsedEvent.key << " " << parsedEvent.my << " " << parsedEvent.mx << " " << (uint32_t)parsedEvent.mbutton << std::endl;
    }
}