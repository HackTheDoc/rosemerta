#include "include/RegisterPage.h"
#include "include/Window.h"
#include "include/Manager.h"
#include "include/Database.h"
#include "include/UI.h"

#include <regex>
#include <filesystem>
#include <iostream>

RegisterPage::RegisterPage() {
    rect = {0, 64, Window::screen.w, Window::screen.h - 64};
}

RegisterPage::~RegisterPage() {}

void RegisterPage::init() {
    border.w = 288;
    border.h = 336;
    border.x = (rect.w - border.w) / 2;
    border.y = 64 + (rect.h - border.h) / 2;

    title = new UILabel("Who am I?", 256, "subtitle");
    title->place(
        border.x + (border.w - title->width()) / 2, 
        border.y + 16
    );

    usernameInput = new InputField("Username", 192, 48);
    usernameInput->place(
        border.x + (border.w - usernameInput->width()) / 2,
        title->y() + title->height()
    );

    passwordInput = new InputField("Password", 192, 48);
    passwordInput->place(
        border.x + (border.w - passwordInput->width()) / 2,
        usernameInput->y() + usernameInput->height() + 16
    );
    
    confirmPasswordInput = new InputField("Confirm", 192, 48);
    confirmPasswordInput->place(
        border.x + (border.w - confirmPasswordInput->width()) / 2,
        passwordInput->y() + passwordInput->height() + 16
    );
    
    button = new UIButton("Register", "valid page", 144, 48);
    button->place(
        border.x + (border.w - button->width()) / 2,
        confirmPasswordInput->y() + confirmPasswordInput->height() + 16
    );
    
    Window::loggedIn = false;
}

void RegisterPage::update() {
    if (Manager::event->enterKeyPressed()) {
        button->use();
        return;
    }
    
    if (Manager::event->tabKeyPressed()) {
        if (usernameInput->selected()) {
            usernameInput->unselect();
            passwordInput->select();
        }
        else if (passwordInput->selected()) {
            passwordInput->unselect();
            confirmPasswordInput->select();
        }
        else if (confirmPasswordInput->selected()) {
            confirmPasswordInput->unselect();
            usernameInput->select();
        }
        else
            usernameInput->select();
    }

    usernameInput->update();
    passwordInput->update();
    confirmPasswordInput->update();
    button->update();
}

void RegisterPage::render() {
    TextureManager::DrawFilledRect(&border, "white");
    title->draw();
    usernameInput->draw();
    passwordInput->draw();
    confirmPasswordInput->draw();
    button->draw();
    TextureManager::DrawRect(&border, "border", 2);
}

void RegisterPage::destroy() {
    title->destroy();
    usernameInput->destroy();
    passwordInput->destroy();
    confirmPasswordInput->destroy();
    button->destroy();
}

void RegisterPage::valid() {
    // check inputs
    if (passwordInput->input.empty() || confirmPasswordInput->input.empty() ||
        passwordInput->input != confirmPasswordInput->input
    ) {
        std::cout << "passwords does not match" << std::endl;
        passwordInput->input = "";
        confirmPasswordInput->input = "";
        return;
    }
    
    std::regex pattern("^[a-zA-Z0-9_-]{4,16}$");
    if (!std::regex_search(usernameInput->input, pattern)) {
        std::cout << "Invalid username!" << std::endl;
        std::cout << "The username must consist of 4 to 16 characters and can only contain letters (both lowercase and uppercase), numbers, underscores, and hyphens." << std::endl;
        usernameInput->input = "";
        return;
    }

    std::string path = "./users/" + usernameInput->input;
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        std::cout << "Username already taken!" << std::endl;
        usernameInput->input = "";
        return;
    }

    // create directories
    std::filesystem::create_directory(path);
    std::filesystem::create_directory(path + "/pictures");
    Database::SetPath(path + "/database.db");

    Database::Create();

    // log in
    Manager::user = std::make_pair(usernameInput->input, passwordInput->input);
    Window::loggedIn = true;
    Window::ui->openPage(Page::Type::BLANK);
}
