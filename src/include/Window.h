#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>

#include "UIElement.h"
#include "Page.h"

class Window {
public:
    static const int DEFAULT_FONT_SIZE;
    static const int TITLE_FONT_SIZE;
    static const int SUBTITLE_FONT_SIZE;
    static const int COMMENTARY_FONT_SIZE;
    
    SDL_Window* w;
    SDL_Renderer* renderer;
    SDL_Rect screen;

    Window();
    ~Window();

    void init();
    void update();
    void render();
    void destroy();

    void addElement(std::string tag, UIElement* elt);
    void removeElement(std::string tag);

    void openPage(Page::Type p);
    void validPage();

    void setColor(std::string tag);
    void setColor(SDL_Color c);
    SDL_Color getColor(std::string tag);

    TTF_Font* getFont(std::string tag);

private:
    std::map<std::string, UIElement*> elements;
    Page* currentPage;

    std::map<std::string, SDL_Color> color;
    std::map<std::string, TTF_Font*> font;
};
