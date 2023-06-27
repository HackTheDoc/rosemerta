#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "UIElement.h"

class UILabel : public UIElement {
public:
    UILabel(std::string text, std::string type="default");
    UILabel(std::string text, int w, std::string type="default");

    void setColor(std::string color);

    void draw() override;
    void destroy() override;

protected:
    SDL_Texture* texture;
    std::string text;
    std::string type;
};
