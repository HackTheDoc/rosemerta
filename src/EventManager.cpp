#include "include/EventManager.h"
#include "include/Window.h"
#include "include/Manager.h"
#include "include/UI.h"

#include <iostream>

std::map<std::string, EventManager::EventID> EventManager::id = {
    {"valid page"           , EventID::VALID                },
    {"log"                  , EventID::LOG                  },
    {"open register page"   , EventID::OPEN_REGISTER_PAGE   },
    {"save"                 , EventID::SAVE                 },
    {"new identity"         , EventID::CREATE_NEW_IDENTITY  },
    {"catalog"              , EventID::CATALOG              },
    {"options"              , EventID::OPTIONS              },
    {"save identity"        , EventID::SAVE_IDENTITY        }
};

EventManager::EventManager() {}

EventManager::~EventManager() {}

void EventManager::handleClick(std::string event) {
    switch(id[event]) {
    case EventID::VALID:
        Window::ui->validPage();
        break;
    case EventID::LOG:
        if (Manager::database != "unknown")
            Manager::database = "unknown";
        Window::ui->openPage(Page::Type::LOGIN);
        break;
    case EventID::OPEN_REGISTER_PAGE:
        Window::ui->openPage(Page::Type::REGISTER);
        break;
    case EventID::SAVE:
        std::cout << "saving..." << std::endl;
        break;
    case EventID::CREATE_NEW_IDENTITY:
        std::cout << "create new identity..." << std::endl;
        break;
    case EventID::CATALOG:
        std::cout << "catalog..." << std::endl;
        break;
    case EventID::OPTIONS:
        std::cout << "options..." << std::endl;
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