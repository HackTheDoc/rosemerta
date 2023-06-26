#include "include/Manager.h"
#include "include/Window.h"

Manager::Manager() {
    // colors
    color["black"]      = {  0,   0,   0, 255};
    color["background"] = {204, 255, 204, 255};

    // fonts
    font["default"]     = TTF_OpenFont("../assets/fonts/Oxanium-Regular.ttf", 16);
    font["title"]       = TTF_OpenFont("../assets/fonts/Oxanium-Bold.ttf", 64);
    font["subtitle"]    = TTF_OpenFont("../assets/fonts/Oxanium-SemiBold.ttf", 32);
}

Manager::~Manager() {}

void Manager::clear() {
    color.clear();

    for (auto f : font) {
        TTF_CloseFont(f.second);
    }
    font.clear();
}

void Manager::setColor(std::string tag) {
    setColor(getColor(tag));
}

void Manager::setColor(SDL_Color c) {
    SDL_SetRenderDrawColor(Window::renderer, c.r, c.g, c.b, c.a);
}

SDL_Color Manager::getColor(std::string tag) {
    if (color.count(tag) <= 0) return color["black"];
    return color[tag];
}

TTF_Font* Manager::getFont(std::string tag) {
    if (font.count(tag) <= 0) return nullptr;
    return font[tag];
}

void Manager::handleKeyboardInput() {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_q:
            if (SDL_GetModState() & KMOD_CTRL)
                Window::isRunning = false;
            break;
        default:
            break;
        }
    }

    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
        default:
            break;
        }
    }
}
