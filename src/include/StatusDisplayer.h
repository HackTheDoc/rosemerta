#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "UIElement.h"

class StatusDisplayer : public UIElement {
public:
    StatusDisplayer(int s);
    ~StatusDisplayer();

    void draw() override;
    void destroy() override;

private:
    SDL_Texture* status;
};
