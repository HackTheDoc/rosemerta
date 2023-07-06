#include "include/Header.h"
#include "include/Application.h"
#include "include/TextureManager.h"

Header::Header() {
    rect = {0, 0, Application::window->screen.w, 64};
}

Header::~Header() {}

void Header::init() {
    border = {rect.x, rect.h - 1, rect.w, 3};

    title = new UILabel("Persona", "title");
    title->place(rect.w - title->width(), 4);
    
    Icon* i = new Icon("./assets/icons/login.png", "Login/Logout", "log");
    i->place(4, (rect.h - i->height()) / 2);
    icons.push_back(i);
    
    i = new Icon("./assets/icons/save.png", "Save", "save");
    i->place((i->width() + 4) * 1 + 4, (rect.h - i->height()) / 2);
    icons.push_back(i);

    i = new Icon("./assets/icons/new-identity.png", "Create New Identity", "new identity");
    i->place((i->width() + 4) * 2 + 4, (rect.h - i->height()) / 2);
    icons.push_back(i);

    i = new Icon("./assets/icons/catalog.png", "Open Catalog", "catalog");
    i->place((i->width() + 4) * 3 + 4, (rect.h - i->height()) / 2);
    icons.push_back(i);
    
    i = new Icon("./assets/icons/settings.png", "Settings", "options");
    i->place((i->width() + 4) * 4 + 4, (rect.h - i->height()) / 2);
    icons.push_back(i);
}

void Header::update() {
    for (int i = 0; i < (int)icons.size(); i++) {
        Icon* ic = icons.at(i);
        ic->update();
        if (ic->hovering) {
            icons.erase(icons.begin() + i);
            icons.insert(icons.end(), ic);
        }
    }
}

void Header::draw() {
    TextureManager::DrawFilledRect(&border, "border");
    
    title->draw();
    
    for (int i = 0; i < (int)icons.size(); i++) {
        Icon* ic = icons.at(i);
        ic->draw();
    }
}

void Header::destroy() {
    title->destroy();
    for (auto i : icons) {
        i->destroy();
    }
}
