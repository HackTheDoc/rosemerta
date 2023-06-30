#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "UIElement.h"

class StatusSelector : public UIElement {
public:
    StatusSelector(int w, int h);
    ~StatusSelector();

    void update() override;
    void draw() override;
    void destroy() override;

    std::string getSelection();

private:
    std::vector<SDL_Texture*> status;
    int selected;
    SDL_Rect textRect;

    SDL_Texture* comment;
    SDL_Rect commentRect, commentBorder;
    bool hovering;
};
