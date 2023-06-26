#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Manager;

class Window {
public:
    static std::string title;
    static SDL_Renderer* renderer;
    static SDL_Rect screen;
    static Manager* manager;
    static bool isRunning;

    Window();
    ~Window();

    int init();
    void handleEvents();
    void update();
    void render();
    void kill();

private:
    SDL_Window* window;
};
