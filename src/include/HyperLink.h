#pragma once

#include <string>

#include "UIElement.h"

class HyperLink : public UIElement {
public:
    HyperLink(std::string text, std::string action);
    ~HyperLink();

    void update() override;
    void draw() override;
    void destroy() override;

private:
    SDL_Texture* normal;
    SDL_Texture* hover;
    std::string action;
    bool hovered;
};
