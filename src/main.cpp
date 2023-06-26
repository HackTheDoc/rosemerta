#include <iostream>
#include <SDL2/SDL.h>

#include "include/Window.h"

#define FPS         60
#define FRAME_DELAY 1000 / FPS

Window window;

int main() {
    Uint32 frameStart;
    int frameTime;

    if (window.init() != 0) return -1;

    while (Window::isRunning) {
        frameStart = SDL_GetTicks();

        window.handleEvents();
        window.update();
        window.render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) SDL_Delay(FRAME_DELAY - frameTime);
    }

    window.kill();

    return 0;
}
