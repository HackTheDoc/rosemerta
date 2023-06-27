#include <iostream>
#include <SDL2/SDL.h>

#include "include/Window.h"

Window window;

int main() {
    if (window.init() != 0) {
        window.kill();
        return -1;
    }

    while (Window::isRunning) {
        window.handleEvents();
        window.update();
        window.render();
    }

    window.kill();

    return 0;
}
