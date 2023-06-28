#include "include/UI.h"
#include "include/Window.h"

UI::UI() {}

UI::~UI() {}

void UI::init() {
    Header* h = new Header();
    h->init();
    elements["header"] = h;

    IdentityEditorPage* p = new IdentityEditorPage();
    elements["identity editor"] = p;
}

void UI::update() {
    for (auto e : elements) {
        e.second->update();
    }
}

void UI::render() {
    for (auto e : elements) {
        e.second->draw();
    }
}

void UI::destroy() {
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
