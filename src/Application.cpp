#include "include/Application.h"

#include <iostream>

std::string Application::title = "Persona";
bool Application::isRunning = false;
bool Application::loggedIn = false;
EventManager* Application::event = nullptr;
Window* Application::window = nullptr;
std::pair<std::string, std::string> Application::user = std::make_pair("-1", "-1");
int Application::selectedIdentity = -1;

Application::Application() {}

Application::~Application() {}

int Application::init() {
    window = new Window();
    event = new EventManager();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << SDL_GetError() << std::endl;
        return -1;
    }
    if (TTF_Init() < 0) {
        std::cout << SDL_GetError() << std::endl;
        return -1;
    }

    window->w = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window->screen.w,
        window->screen.h,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cout << "Error creating window!" << std::endl;
        return -2;
    }

    window->renderer = SDL_CreateRenderer(
        window->w,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!window->renderer) {
        std::cout << "Error creating renderer!" << std::endl;
        return -2;
    }

    window->init();
    window->setColor("background");

    isRunning = true;

    return 0;
}

void Application::handleEvents() {
    SDL_PollEvent(&event->e);

    switch (event->e.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        event->handleKeyboardInput();
        break;
    }
}

void Application::update() {
    window->update();
}

void Application::render() {
    SDL_RenderClear(window->renderer);

    window->render();

    SDL_RenderPresent(window->renderer);
}

void Application::kill() {
    delete event;
    event = nullptr;

    window->destroy();
    delete window;
    window = nullptr;

    SDL_Quit();
}
