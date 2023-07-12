#pragma once

#include <string>

#include "UIElement.h"

class UIButton : public UIElement {
public:
    UIButton(std::string text, std::string action);
    UIButton(std::string text, std::string action, int w, int h);
    ~UIButton();

    void update() override;
    void draw() override;
    void destroy() override;

    void use();
    void place(int x, int y);

private:
    SDL_Texture* text;
    SDL_Rect textRect;
    std::string action;
    bool hovered;
};
