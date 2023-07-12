#include "include/EventManager.h"
#include "include/Application.h"

#include <iostream>

std::map<std::string, EventManager::EventID> EventManager::id = {
    {"valid page"           , EventID::VALID                },
    {"log"                  , EventID::LOG                  },
    {"open register page"   , EventID::OPEN_REGISTER_PAGE   },
    {"save"                 , EventID::SAVE                 },
    {"identity"             , EventID::IDENTITY             },
    {"new identity"         , EventID::CREATE_NEW_IDENTITY  },
    {"catalog"              , EventID::CATALOG              },
    {"options"              , EventID::OPTIONS              },
    {"save identity"        , EventID::SAVE_IDENTITY        }
};

EventManager::EventManager() {}

EventManager::~EventManager() {}

void EventManager::handleClick(std::string event) {
    if (Application::loggedIn)
        handleLoggedInClicks(event);
    else
        handleLoggedOffClicks(event);
}

void EventManager::handleKeyboardInput() {
    if (e.type != SDL_KEYUP) return;

    if (Application::loggedIn)
        handleLoggedInEvents();
    else
        handleLoggedOffEvents();
}

bool EventManager::enterKeyPressed() {
    return e.type == SDL_KEYUP && (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_RETURN2);
}

bool EventManager::tabKeyPressed() {
    return e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_TAB;
}

bool EventManager::mouseClickLeft() {
    return e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT;
}

bool EventManager::mouseClickRight() {
    return e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT;
}

bool EventManager::mouseIn(const SDL_Rect& rect) {
    int x,y;
    SDL_GetMouseState(&x, &y);
    if (x >= rect.x && x <= rect.x + rect.w &&
        y >= rect.y && y <= rect.y + rect.h
    ) {
        return true;
    }
    else return false;
}

bool EventManager::mouseClickLeftIn(const SDL_Rect& rect) {
    return mouseIn(rect) && mouseClickLeft();
}

void EventManager::handleLoggedInClicks(std::string event) {
    switch(id[event]) {
    case EventID::LOG:
        Application::LogOff();
        break;
    case EventID::SAVE:
        std::cout << "saving..." << std::endl;
        break;
    case EventID::IDENTITY:
        std::cout << "inspecting identity " << Application::selectedIdentity << " ..." << std::endl;
        break;
    case EventID::CREATE_NEW_IDENTITY:
        Application::window->openPage(Page::Type::IDENTITY_EDITOR);
        break;
    case EventID::CATALOG:
        Application::window->openPage(Page::Type::CATALOG);
        break;
    case EventID::OPTIONS:
        std::cout << "options..." << std::endl;
        break;
    default:
        break;
    }
}

void EventManager::handleLoggedOffClicks(std::string event) {
    switch(id[event]) {
    case EventID::VALID:
        Application::window->validPage();
        break;
    case EventID::LOG:
            Application::window->openPage(Page::Type::LOGIN);
        break;
    case EventID::OPEN_REGISTER_PAGE:
            Application::window->openPage(Page::Type::REGISTER);
        break;
    default:
        break;
    }
}

void EventManager::handleLoggedInEvents() {
    switch (e.key.keysym.sym) {
    case SDLK_q:
        if (SDL_GetModState() & KMOD_CTRL)
            Application::isRunning = false;
        break;
    case SDLK_n:
        if (SDL_GetModState() & KMOD_CTRL) {
            Application::selectedIdentity = -1;
            Application::window->openPage(Page::Type::IDENTITY_EDITOR);
        }
    case SDLK_c:
        if (SDL_GetModState() & KMOD_CTRL)
            Application::window->openPage(Page::Type::CATALOG);
    case SDLK_ESCAPE:
        Application::selectedIdentity = -1;
    default:
        break;
    }
}

void EventManager::handleLoggedOffEvents() {
    switch(e.key.keysym.sym) {
    case SDLK_q:
        if (SDL_GetModState() & KMOD_CTRL)
            Application::isRunning = false;
        break;
    case SDLK_l:
        if (SDL_GetModState() & KMOD_CTRL) {
            Application::selectedIdentity = -1;
            Application::window->openPage(Page::Type::LOGIN);
        }
    case SDLK_r:
        if (SDL_GetModState() & KMOD_CTRL) {
            Application::selectedIdentity = -1;
            Application::window->openPage(Page::Type::REGISTER);
        }
    default:
        break;
    }
}
