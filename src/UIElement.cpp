#include "include/UIElement.h"

void UIElement::place(int x, int y) {
    rect.x = x;
    rect.y = y;
}

void UIElement::getPosition(int* x, int* y) {
    *x = rect.x;
    *y = rect.y;
}

int UIElement::width() {
    return rect.w;
}

int UIElement::height() {
    return rect.h;
}

int UIElement::x() {
    return rect.x;
}

int UIElement::y() {
    return rect.y;
}
