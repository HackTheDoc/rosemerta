#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>

#include "TextureManager.h"
#include "EventManager.h"
#include "Window.h"

class Application {
public:    
    static std::string title;
    static bool isRunning;
    static bool loggedIn;
    static EventManager* event;
    static Window* window;
    static std::pair<std::string, std::string> user;
    static int selectedIdentity;

    Application();
    ~Application();

    int init();
    void handleEvents();
    void update();
    void render();
    void kill();

    static void LogOff();
};
