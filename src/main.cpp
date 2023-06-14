#include <iostream>
#include <SDL2/SDL.h>

#include "include/Application.h"

Application app;

int main() {
    app.init();

    app.run();

    app.kill();

    return 0;
}
