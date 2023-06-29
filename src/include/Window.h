#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Manager;
class UI;

class Window {
public:
    static std::string title;
    static SDL_Renderer* renderer;
    static SDL_Rect screen;
    static Manager* manager;
    static UI* ui;
    static bool isRunning;
    static bool loggedIn;

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
