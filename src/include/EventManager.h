#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <map>

class EventManager {
public:
    enum EventID {
        LOG,
        REGISTER,
        SAVE, // is it really usefull ?
        CREATE_NEW_IDENTITY,
        CATALOG,
        OPTIONS,
        SAVE_IDENTITY
    };

    SDL_Event e;
    
    void handleClick(std::string event);

    void handleKeyboardInput();

    bool enterKeyPressed();

    bool mouseClickLeft();
    bool mouseClickRight();

    bool mouseClickLeftIn(const SDL_Rect& rect);

private:
    static const std::map<std::string, EventID> id;
};
