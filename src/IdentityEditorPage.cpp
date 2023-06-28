#include "include/IdentityEditorPage.h"
#include "include/Window.h"
#include "include/Manager.h"

#include <iostream>

IdentityEditorPage::IdentityEditorPage() {
    rect = {0, 64, Window::screen.w, Window::screen.h - 64};

    profilePicture = new ImageDisplayer(".app/assets/pictures/unknown.png", 160, 160);
    profilePicture->place(rect.x + 32, rect.y + 32);

    usernameInput = new InputField("Username", 192, 48);
    usernameInput->place(
        profilePicture->x() + profilePicture->width() + 32,
        profilePicture->y() + 11
    );
    
    ageInput = new InputField("Age", 192, 48);
    ageInput->place(
        usernameInput->x(),
        usernameInput->y() + usernameInput->height() + 12
    );

    saveButton = new UIButton("Save", "save identity", 128, 48);
    saveButton->place(
        rect.x + rect.w - saveButton->width() - 32,
        rect.y + rect.h - saveButton->height() - 32
    );
}

IdentityEditorPage::~IdentityEditorPage() {}

void IdentityEditorPage::update() {
    usernameInput->update();
    ageInput->update();
    saveButton->update();

    if (Manager::event->enterKeyPressed()) {
        if (!usernameInput->input.empty()) {
            std::cout << usernameInput->input << std::endl;
            usernameInput->input = "";
        }
        if (!ageInput->input.empty()) {
            std::cout << ageInput->input << std::endl;
            ageInput->input = "";
        }
    }
}

void IdentityEditorPage::draw() {
    profilePicture->draw();
    usernameInput->draw();
    ageInput->draw();
    saveButton->draw();
}

void IdentityEditorPage::destroy() {
    profilePicture->destroy();
    usernameInput->destroy();
    ageInput->destroy();
    saveButton->destroy();
}
