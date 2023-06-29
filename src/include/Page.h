#pragma once

#include <SDL2/SDL.h>

class Page {
public:
    enum Type {
        BLANK,
        LOGIN,
        REGISTER
    };

    Page() = default;
    ~Page() = default;
    
    virtual void init() {}
    virtual void update() {}
    virtual void render() {}
    virtual void destroy() {}
    virtual void valid() {}

    void place(int x, int y);

    void getPosition(int* x, int* y);
    int width();
    int height();
    int x();
    int y();

protected:
    SDL_Rect rect;
};
