#include <iostream>
#include <SDL2/SDL.h>

#include "include/Application.h"

Application app;

int main() {
    if (app.init() != 0) {
        app.kill();
        return -1;
    }

    while (Application::isRunning) {
        app.handleEvents();
        app.update();
        app.render();
    }

    app.kill();

    return 0;
}
