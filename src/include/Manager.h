#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>

#include "TextureManager.h"

class Window;

class Manager {
public:
    SDL_Event event;

    Manager();
    ~Manager();

    void clear();

    void setColor(std::string tag);
    void setColor(SDL_Color c);
    SDL_Color getColor(std::string tag);

    TTF_Font* getFont(std::string tag);

    void handleKeyboardInput();

private:
    std::map<std::string, SDL_Color> color;
    std::map<std::string, TTF_Font*> font;
};