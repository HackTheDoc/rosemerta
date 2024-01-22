#pragma once

#include <string>

namespace hue {
    enum color {
        BLACK,
        RED,
        GREEN,
        ORANGE,
        BLUE,
        MAGENTA,
        CYAN,
        LIGHT_GRAY,
        DEFAULT
    };

    void print(const char* msg, const color c = color::DEFAULT);

    void println(const char* msg, const color c = color::DEFAULT);
}
