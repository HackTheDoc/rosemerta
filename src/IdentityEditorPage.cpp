#include "include/IdentityEditorPage.h"
#include "include/Window.h"
#include "include/Manager.h"

#include <iostream>

IdentityEditorPage::IdentityEditorPage() {
    rect = {0, 64, Window::screen.w, Window::screen.h - 64};
}

IdentityEditorPage::~IdentityEditorPage() {}

void IdentityEditorPage::init() {
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

    statusSelector = new StatusSelector(192, 48);
    statusSelector->place(
        rect.w - statusSelector->width() - 32,
        usernameInput->y()
    );

    birthdayInput = new DateInput(256, 48);
    birthdayInput->place(
        rect.w - birthdayInput->width() - 32,
        ageInput->y()
    ); 

    saveButton = new UIButton("Save", "save identity", 128, 48);
    saveButton->place(
        rect.x + rect.w - saveButton->width() - 32,
        rect.y + rect.h - saveButton->height() - 32
    );
}

void IdentityEditorPage::update() {
    if (Manager::event->enterKeyPressed()) {
        if (!usernameInput->input.empty()) {
            std::cout << usernameInput->input << std::endl;
            usernameInput->input = "";
        }
        if (!ageInput->input.empty()) {
            std::cout << ageInput->input << std::endl;
            ageInput->input = "";
        }
        std::cout << birthdayInput->getDate() << std::endl;
        birthdayInput->clear();
    }

    if (Manager::event->tabKeyPressed()) {
        if (usernameInput->selected()) {
            usernameInput->unselect();
            ageInput->select();
        }
        else if (ageInput->selected()) {
            ageInput->unselect();
            birthdayInput->selectDay();
        }
        else if (birthdayInput->selectedDay()) {
            birthdayInput->unselectDay();
            birthdayInput->selectMonth();
        }
        else if (birthdayInput->selectedMonth()) {
            birthdayInput->unselectMonth();
            birthdayInput->selectYear();
        }
        else if (birthdayInput->selectedYear()) {
            birthdayInput->unselectYear();
            usernameInput->select();
        }
        else
            usernameInput->select();
    }

    usernameInput->update();
    ageInput->update();
    statusSelector->update();
    birthdayInput->update();
    saveButton->update();
}

void IdentityEditorPage::render() {
    profilePicture->draw();
    usernameInput->draw();
    ageInput->draw();
    statusSelector->draw();
    birthdayInput->draw();
    saveButton->draw();
}

void IdentityEditorPage::destroy() {
    profilePicture->destroy();
    usernameInput->destroy();
    ageInput->destroy();
    statusSelector->destroy();
    birthdayInput->destroy();
    saveButton->destroy();
}
