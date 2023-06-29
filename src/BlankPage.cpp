#include "include/BlankPage.h"
#include "include/Window.h"
#include "include/Manager.h"

BlankPage::BlankPage() {
    rect = {0, 64, Window::screen.w, Window::screen.h - 64};
}

BlankPage::~BlankPage() {}

void BlankPage::init() {
    lbl = new UILabel("Welcome, "+Manager::user.first+" !", "subtitle");
    lbl->place(
        (rect.w - lbl->width()) / 2,
        rect.y + 16
    );
}

void BlankPage::render() {
    lbl->draw();
}

void BlankPage::destroy() {
    lbl->destroy();
}
