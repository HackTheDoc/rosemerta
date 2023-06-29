#include "include/UI.h"
#include "include/Window.h"

UI::UI() : currentPage(nullptr) {}

UI::~UI() {}

void UI::init() {
    Header* h = new Header();
    h->init();
    elements["header"] = h;

    openPage(Page::Type::LOGIN);
}

void UI::update() {
    currentPage->update();
    for (auto e : elements) {
        e.second->update();
    }
}

void UI::render() {
    currentPage->render();
    for (auto e : elements) {
        e.second->draw();
    }
}

void UI::destroy() {
    currentPage->destroy();
    for (auto e : elements) {
        e.second->destroy();
    }
    elements.clear();
}

void UI::add(std::string tag, UIElement* elt) {
    remove(tag);
    elements[tag] = elt;
}

void UI::remove(std::string tag) {
    if (elements.count(tag) == 0) return;
    elements[tag]->destroy();
    elements.erase(tag);
}

void UI::openPage(Page::Type p) {
    if (!Window::loggedIn) {
        if (p == Page::Type::LOGIN) {
            if (currentPage != nullptr) currentPage->destroy();
            currentPage = new LoginPage();
            currentPage->init();
        }
        else if (p == Page::Type::REGISTER) {
            if (currentPage != nullptr) currentPage->destroy();
            currentPage = new RegisterPage();
            currentPage->init();
        }
        return;
    }

    if (currentPage != nullptr) currentPage->destroy();

    switch (p) {
    case Page::Type::LOGIN:
        currentPage = new LoginPage();
        break;
    case Page::Type::REGISTER:
        currentPage = new RegisterPage();
        break;
    case Page::Type::IDENTITY_EDITOR:
        currentPage = new IdentityEditorPage();
        break;
    case Page::Type::BLANK:
    default:
        currentPage = new Page();
        break;
    }

    currentPage->init();
}

void UI::validPage() {
    currentPage->valid();
}
