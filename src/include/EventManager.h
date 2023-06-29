#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <map>

class EventManager {
public:
    enum EventID {
        VALID,
        LOG,
        OPEN_REGISTER_PAGE,
        SAVE, // is it really usefull ?
        CREATE_NEW_IDENTITY,
        CATALOG,
        OPTIONS,
        SAVE_IDENTITY
    };

    SDL_Event e;
    
    EventManager();
    ~EventManager();

    void handleClick(std::string event);

    void handleKeyboardInput();

    bool enterKeyPressed();
    bool tabKeyPressed();

    bool mouseClickLeft();
    bool mouseClickRight();

    bool mouseClickLeftIn(const SDL_Rect& rect);

private:
    static std::map<std::string, EventID> id;
};
