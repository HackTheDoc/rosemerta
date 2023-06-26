#include "include/Window.h"
#include "include/Manager.h"

#include <iostream>

std::string Window::title = "Persona";
SDL_Renderer* Window::renderer = nullptr;
SDL_Rect Window::screen = {0, 0, 800, 600};
Manager* Window::manager = nullptr;
bool Window::isRunning = false;

Window::Window() : window(nullptr) {}

Window::~Window() {}

int Window::init() {
    manager = new Manager();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << SDL_GetError() << std::endl;
        return -1;
    }

    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screen.w,
        screen.h,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cout << "Error creating window!" << std::endl;
        return -2;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::cout << "Error creating renderer!" << std::endl;
        return -2;
    }
    manager->setColor("background");

    if (TTF_Init() < 0) {
        std::cout << SDL_GetError() << std::endl;
        return -1;
    }

    isRunning = true;

    return 0;
}

void Window::handleEvents() {
    SDL_PollEvent(&manager->event);

    switch (manager->event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        manager->handleKeyboardInput();
        break;
    }
}

void Window::update() {
    // nothing to update
}

void Window::render() {
    SDL_RenderClear(renderer);

    // nothing to render

    SDL_RenderPresent(renderer);
}

void Window::kill() {
    manager->clear();
    delete manager;
    manager = nullptr;

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}
