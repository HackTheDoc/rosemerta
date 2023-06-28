#pragma once

#include <SDL2/SDL.h>

#include "UIElement.h"

class ImageDisplayer : public UIElement {
public:
    ImageDisplayer(const char* imagepath, int w = 64, int h = 64);
    ~ImageDisplayer();

    void setImage(const char* imagepath);

    void draw() override;
    void destroy() override;

private:
    SDL_Texture* img;
};
