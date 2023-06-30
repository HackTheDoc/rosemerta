#include "include/LoginPage.h"
#include "include/Window.h"
#include "include/Manager.h"
#include "include/Database.h"
#include "include/UI.h"

#include <regex>
#include <filesystem>
#include <iostream>

LoginPage::LoginPage() {
    rect = {0, 64, Window::screen.w, Window::screen.h - 64};
}

LoginPage::~LoginPage() {}

void LoginPage::init() {
    border.w = 288;
    border.h = 292;
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
    
    button = new UIButton("Login", "valid page", 128, 48);
    button->place(
        border.x + (border.w - button->width()) / 2,
        passwordInput->y() + passwordInput->height() + 16
    );

    link = new HyperLink("or register", "open register page");
    link->place(
        border.x + (border.w - link->width()) / 2,
        button->y() + button->height() + 8
    );

    // log off if already logged in
    Manager::database = "unknown";
    Manager::user = std::make_pair("unknown", "unknown");
    Window::loggedIn = false;
}

void LoginPage::update() {
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
            usernameInput->select();
        } else {
            usernameInput->select();
        }
    }

    usernameInput->update();
    passwordInput->update();
    link->update();
    button->update();
}

void LoginPage::render() {
    TextureManager::DrawFilledRect(&border, "white");
    title->draw();
    usernameInput->draw();
    passwordInput->draw();
    link->draw();
    button->draw();
    TextureManager::DrawRect(&border, "border", 2);
}

void LoginPage::destroy() {
    title->destroy();
    usernameInput->destroy();
    passwordInput->destroy();
    link->destroy();
    button->destroy();
}

void LoginPage::valid() {
    // check inputs
    if (passwordInput->input.empty()) {
        std::cout << "passwords not specified" << std::endl;
        passwordInput->input = "";
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
    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        std::cout << "User " << usernameInput->input << " doesn't exist!" << std::endl;
        usernameInput->input = "";
        return;
    }

    // saving user login data
    Database::SetPath(path+"/database.db");
    Manager::user = std::make_pair(usernameInput->input, passwordInput->input);
    Window::loggedIn = true;

    // log in
    //Manager::Decrypt("./app/users/"+Manager::user.first, Manager::user.second);
    Window::ui->openPage(Page::Type::BLANK);
}
