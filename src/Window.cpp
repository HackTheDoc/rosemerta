#include "include/Window.h"
#include "include/Application.h"

#include "include/Header.h"

#include "include/BlankPage.h"
#include "include/RegisterPage.h"
#include "include/LoginPage.h"
#include "include/IdentityEditorPage.h"

#include <iostream>

const int Window::DEFAULT_FONT_SIZE    = 32;
const int Window::TITLE_FONT_SIZE      = 64;
const int Window::SUBTITLE_FONT_SIZE   = 48;
const int Window::COMMENTARY_FONT_SIZE = 16;

Window::Window() : w(nullptr), renderer(nullptr), screen({0,0,800,600}), currentPage(nullptr) {}

Window::~Window() {}

void Window::init() {
    // colors
    color["black"]          = {  0,   0,   0, 255};
    color["white"]          = {255, 255, 255, 255};
    color["red"]            = {255,   0,   0, 255};
    color["green"]          = {  0, 255,   0, 255};
    color["blue"]           = {128, 191, 255, 255};
    color["dark blue"]      = { 26, 140, 255, 255};
    color["pink"]           = {230,   0, 230, 255};
    color["turquoise"]      = {  0, 204, 204, 255};
    color["gray"]           = {127, 127, 127, 255};
    color["light gray"]     = {192, 192, 192, 255};
    color["background"]     = {229, 252, 245, 255}; // mint green
    color["border"]         = {117,  13,  55, 255}; // claret
    color["font"]           = { 33,   1,  36, 255}; // dark purple

    // fonts
    font["default"]         = TTF_OpenFont("./assets/fonts/Oxanium-Regular.ttf" , DEFAULT_FONT_SIZE);
    font["big"]             = TTF_OpenFont("./assets/fonts/Oxanium-Regular.ttf" , SUBTITLE_FONT_SIZE);
    font["title"]           = TTF_OpenFont("./assets/fonts/Oxanium-Bold.ttf"    , TITLE_FONT_SIZE);
    font["subtitle"]        = TTF_OpenFont("./assets/fonts/Oxanium-SemiBold.ttf", SUBTITLE_FONT_SIZE);
    font["comment"]         = TTF_OpenFont("./assets/fonts/Oxanium-Regular.ttf" , COMMENTARY_FONT_SIZE);

    Header* h = new Header();
    h->init();
    elements["header"] = h;

    openPage(Page::Type::LOGIN);
}

void Window::update() {
    currentPage->update();
    for (auto e : elements) {
        e.second->update();
    }
}

void Window::render() {
    currentPage->render();
    for (auto e : elements) {
        e.second->draw();
    }
}

void Window::destroy() {
    color.clear();

    for (auto f : font) {
        TTF_CloseFont(f.second);
    }
    font.clear();

    currentPage->destroy();
    for (auto e : elements) {
        e.second->destroy();
    }
    elements.clear();

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(w);
    w = nullptr;
}

void Window::addElement(std::string tag, UIElement* elt) {
    removeElement(tag);
    elements[tag] = elt;
}

void Window::removeElement(std::string tag) {
    if (elements.count(tag) == 0) return;
    elements[tag]->destroy();
    elements.erase(tag);
}

void Window::openPage(Page::Type p) {
    if (!Application::loggedIn) {
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
        currentPage = new BlankPage();
        break;
    }

    currentPage->init();
}

void Window::validPage() {
    currentPage->valid();
}

void Window::setColor(std::string tag) {
    setColor(getColor(tag));
}

void Window::setColor(SDL_Color c) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

SDL_Color Window::getColor(std::string tag) {
    if (color.count(tag) <= 0) return color["black"];
    return color[tag];
}

TTF_Font* Window::getFont(std::string tag) {
    if (font.count(tag) <= 0) return nullptr;
    return font[tag];
}
