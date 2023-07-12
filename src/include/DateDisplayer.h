#pragma once

#include "UIElement.h"

class DateDisplayer : public UIElement {
public:
    DateDisplayer(int d, int m, int y);
    ~DateDisplayer();

    void draw() override;
    void destroy() override;

private:
    SDL_Texture* texture;
};
