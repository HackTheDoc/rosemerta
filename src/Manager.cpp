#include "include/Manager.h"
#include "include/Window.h"

#include <iostream>

const int Manager::DEFAULT_FONT_SIZE    = 16;
const int Manager::TITLE_FONT_SIZE      = 64;
const int Manager::SUBTITLE_FONT_SIZE   = 32;

Manager::Manager() {}

Manager::~Manager() {}

void Manager::init() {
    // colors
    color["black"]          = {  0,   0,   0, 255};
    color["background"]     = {204, 255, 204, 255};
    color["black purple"]   = {136,   0, 204, 255};

    // fonts
    font["default"]     = TTF_OpenFont("./assets/fonts/Oxanium-Regular.ttf", DEFAULT_FONT_SIZE);
    font["title"]       = TTF_OpenFont("./assets/fonts/Oxanium-Bold.ttf", TITLE_FONT_SIZE);
    font["subtitle"]    = TTF_OpenFont("./assets/fonts/Oxanium-SemiBold.ttf", SUBTITLE_FONT_SIZE);
}

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
