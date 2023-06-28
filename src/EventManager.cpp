#include "include/EventManager.h"
#include "include/Window.h"

const std::map<std::string, EventManager::EventID> EventManager::id = {
    {"log"          , EventID::LOG                  },
    {"register"     , EventID::REGISTER             },
    {"new identity" , EventID::CREATE_NEW_IDENTITY  },
    {"catalog"      , EventID::CATALOG              },
    {"options"      , EventID::OPTIONS              },
    {"save identity", EventID::SAVE_IDENTITY        }
};

void EventManager::handleClick(std::string event) {
    switch(id.at(event)) {
    case EventID::LOG:
        break;
    case EventID::REGISTER:
        break;
    case EventID::CREATE_NEW_IDENTITY:
        break;
    case EventID::CATALOG:
        break;
    case EventID::OPTIONS:
        break;
    default:
        break;
    }
}

void EventManager::handleKeyboardInput() {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_q:
            if (SDL_GetModState() & KMOD_CTRL)
                Window::isRunning = false;
            break;
        default:
            break;
        }
    }

    if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
        default:
            break;
        }
    }
}

bool EventManager::enterKeyPressed() {
    return e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_RETURN;
}

bool EventManager::mouseClickLeft() {
    return e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT;
}

bool EventManager::mouseClickRight() {
    return e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT;
}

bool EventManager::mouseClickLeftIn(const SDL_Rect& rect) {
    if (!mouseClickLeft()) return false;

    int x,y;
    SDL_GetMouseState(&x, &y);
    if (x >= rect.x && x <= rect.x + rect.w &&
        y >= rect.y && y <= rect.y + rect.h
    ) {
        return true;
    }
    else return false;
}
