#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "UIElement.h"

class InputField : public UIElement {
public:
    std::string input;

    InputField(const char* placeholder);
    InputField(const char* placeholder, int w, int h);
    ~InputField();

    void update() override;
    void draw() override;
    void destroy() override;

    void place(int x, int y);

    bool selected();
    void select();
    void unselect();

private:
    static SDL_Rect cursor;
    
    SDL_Rect field, textRect;
    SDL_Texture* placeholder;
    SDL_Texture* text;
    bool actived;
};
