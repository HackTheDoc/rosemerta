#pragma once

#include <SDL2/SDL.h>

#include "UIElement.h"

class CatalogItem : public UIElement {
public:
    CatalogItem(int id, const char* text, SDL_Texture* pic);
    ~CatalogItem();

    void update() override;
    void draw() override;
    void destroy() override;

private:
    static int h;
    
    SDL_Texture* picture;
    SDL_Rect picRect;

    SDL_Texture* name;
    SDL_Rect nameRect;

    int id;
};
