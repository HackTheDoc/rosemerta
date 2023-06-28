#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "UIElement.h"

class Icon : public UIElement {
public:
    bool hovering;
    
    Icon(std::string iconpath, std::string text, std::string action, int x = 0, int y = 0);
    ~Icon();

    void update() override;
    void draw() override;
    void destroy() override;

private:
    SDL_Texture* icon;
    SDL_Texture* comment;
    SDL_Rect commentRect, commentBorder;
    std::string action;
};
