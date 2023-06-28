#include "include/Page.h"

void Page::place(int x, int y) {
    rect.x = x;
    rect.y = y;
}

void Page::getPosition(int* x, int* y) {
    *x = rect.x;
    *y = rect.y;
}

int Page::width() {
    return rect.w;
}

int Page::height() {
    return rect.h;
}

int Page::x() {
    return rect.x;
}

int Page::y() {
    return rect.y;
}