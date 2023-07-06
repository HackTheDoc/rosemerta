#include "include/BlankPage.h"
#include "include/Application.h"
#include "include/TextureManager.h"

BlankPage::BlankPage() {
    rect = {0, 64, Application::window->screen.w, Application::window->screen.h - 64};
}

BlankPage::~BlankPage() {}

void BlankPage::init() {
    lbl = new UILabel("Welcome, "+Application::user.first+" !", "subtitle");
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
