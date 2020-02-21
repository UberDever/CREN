//
// Created by uberdever on 19.02.2020.
//

#include "util.hpp"

namespace util
{
    c_gameEvent parse_event(SDL_Event* event)
    {
        uint32_t eventCodes[8] = {SDL_QUIT, SDL_WINDOWEVENT, SDL_KEYDOWN, SDL_KEYUP, SDL_MOUSEMOTION, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP, SDL_MOUSEWHEEL};
        int index = 0;
        for (; index < 8 && (event->type ^ eventCodes[index]); index++);
        c_gameEvent parsedEvent;
        parsedEvent.key = event->key.keysym.sym;
        parsedEvent.mx = event->motion.x;
        parsedEvent.my = event->motion.x;
        parsedEvent.mstate = event->motion.state;
        std::cout << index << " " << event->motion.x << " " << event->button.x << " " << (event->motion.state & SDL_BUTTON_RMASK) << std::endl;
        std::cout << index << " " << event->motion.y << " " << event->button.y << " " << (event->motion.state & SDL_BUTTON_LMASK) << std::endl << std::endl;
        /*c_gameEvent parsedEvent;
        if (event->type == SDL_KEYDOWN) {
            parsedEvent.key = (e_gameKeys)(event->key.keysym.sym);
        } else if (event->type == SDL_KEYUP) {
            std::cout << SDL_KEYUP << std::endl;
            parsedEvent.key = (e_gameKeys)(event->key.keysym.sym);
            parsedEvent.x = -1; //key is released
        }/* else
        {
            parsedEvent.key = static_cast<e_gameKeys>(event->type);
            parsedEvent.mx = event->motion.x | event->button.x;
            parsedEvent.my = event->motion.y | event->button.y;
            std::cout << parsedEvent.key << std::endl;
            std::cout << "y:" << parsedEvent.my << " x:" << parsedEvent.mx << std::endl;
        }*/
    }
}