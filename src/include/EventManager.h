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
        IDENTITY,
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

    bool mouseIn(const SDL_Rect& rect);
    bool mouseClickLeftIn(const SDL_Rect& rect);

private:
    static std::map<std::string, EventID> id;

    void handleLoggedInEvents();
    void handleLoggedOffEvents();

    void handleLoggedInClicks(std::string event);
    void handleLoggedOffClicks(std::string event);
};
