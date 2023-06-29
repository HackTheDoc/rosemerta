#pragma once

#include <SDL2/SDL.h>

#include "Page.h"
#include "UILabel.h"

class BlankPage : public Page {
public:
    BlankPage();
    ~BlankPage();

    void init() override;
    void render() override;
    void destroy() override;

private:
    UILabel* lbl;
};
