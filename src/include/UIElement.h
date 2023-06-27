#pragma once

#include <SDL2/SDL.h>

class UIElement {
public:
    UIElement() = default;
    ~UIElement() = default;

    virtual void update() {}
    virtual void draw() {}
    virtual void destroy() {}

    void place(int x, int y);

    void getPosition(int* x, int* y);
    int width();
    int height();

protected:
    SDL_Rect rect;
};
