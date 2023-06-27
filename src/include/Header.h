#pragma once

#include "UIElement.h"
#include "UILabel.h"
#include "Icon.h"
#include <vector>

class Header : public UIElement {
public:
    Header();
    ~Header();

    void init();
    void update() override;
    void draw() override;
    void destroy() override;

private:
    SDL_Rect border;
    UILabel* title;
    std::vector<Icon*> icons;
};
