#pragma once

#include <SDL2/SDL.h>

#include "UIElement.h"
#include "UILabel.h"
#include "UIButton.h"
#include "StatusDisplayer.h"
#include "DateDisplayer.h"

class CatalogSelection : public UIElement {
public:
    CatalogSelection(int w, int h);
    ~CatalogSelection();

    void update() override;
    void draw() override;
    void destroy() override;

    void place(int x, int y);

private:    
    SDL_Texture* picture;
    SDL_Rect picRect;

    StatusDisplayer* status;
    UILabel* name;
    UILabel* age;
    DateDisplayer* birthday;

    UIButton* inspectButton;

    int id;
};
