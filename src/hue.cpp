#include "include/hue.h"

#include <iostream>

namespace hue {
    namespace {
        const char* colorCode(const hue::color c) {
            switch (c) {
            case hue::color::BLACK:
                return "30";
            case hue::color::RED:
                return "31";
            case hue::color::GREEN:
                return "32";
            case hue::color::ORANGE:
                return "33";
            case hue::color::BLUE:
                return "34";
            case hue::color::MAGENTA:
                return "35";
            case hue::color::CYAN:
                return "36";
            case hue::color::LIGHT_GRAY:
                return "37";
            case hue::color::DEFAULT:
            default:
                return "39";
            }
        }
    };

    void print(const char* msg, const hue::color c) {
        std::cout << "\033[" << colorCode(c) << "m";
        std::cout << msg;
        std::cout << "\033[0m";
    }

    void println(const char* msg, const hue::color c) {
        print(msg, c);
        std::cout << std::endl;
    }
};
