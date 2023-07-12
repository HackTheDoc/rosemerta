#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "UIElement.h"

class CatalogFilter : public UIElement {
public:
    CatalogFilter(int w, int h);
    ~CatalogFilter();

    void update() override;
    void draw() override;
    void destroy() override;

    void place(int x, int y);

private:
    SDL_Texture* text;
    SDL_Rect textRect;

    SDL_Rect selectorRect;

    std::vector<SDL_Texture*> filters;
    int selected;
    SDL_Rect filterRect;

    SDL_Texture* comment;
    SDL_Rect commentRect, commentBorder;
    bool hovering;
};
