#include <iostream>
#include <docopt/docopt.h>
#include <string>
#include <map>


#include "include/Application.h"

Application app;

const char* USAGE =
R"(Persona.

    Usage:
      persona register
      persona login
      persona (-h | --help)
      persona --version

    Options:
      -h --help     Show this screen.
      --version     Show version.
)";

int main(int argc, const char* argv[]) {
    std::string username = "unknown";
    bool n = false;

    std::map<std::string, docopt::value> args
        = docopt::docopt(USAGE,
                        {argv + 1, argv + argc},
                        true,
                        "Persona 0.2");
    
    if (args["register"].asBool()) {
        n = true;
        std::cout << "  username: ";
        std::cin >> username;
    }
    else if (args["login"].asBool()) {
        std::cout << "  username: ";
        std::cin >> username;
    }

    app.init(username, n);

    app.start();

    while (Application::isRunning) app.run();

    app.kill();

    return 0;
}
