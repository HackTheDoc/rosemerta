#include <iostream>

#include "include/application.h"

Application app;

int main(int argc, const char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "-v" || arg == "--version") {
            std::cout << "rosemerta " + Application::version << std::endl;
            exit(EXIT_SUCCESS);
        }
        else if (arg == "-h" || arg == "--help") {
            std::cout << "Usage:"                                   << std::endl;
            std::cout << "  rosemerta [option]"                     << std::endl;

            std::cout << std::endl;

            std::cout << "Options:"                                 << std::endl;
            std::cout << "  -h or --help    Display this text."     << std::endl;
            std::cout << "  -v or --version Show current version."  << std::endl;

            exit(EXIT_SUCCESS);
        }
    }

    app.start();

    while (Application::isRunning) app.run();

    app.kill();

    exit(EXIT_SUCCESS);
}
