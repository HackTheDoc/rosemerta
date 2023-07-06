#include "include/HyperLink.h"
#include "include/TextureManager.h"
#include "include/Application.h"

HyperLink::HyperLink(std::string text, std::string action) {
    normal = TextureManager::GenerateText(text.c_str(), "comment", 512, "blue");
    hover = TextureManager::GenerateText(text.c_str(), "comment", 512, "dark blue");

    rect.x = rect.y = 0;
    SDL_QueryTexture(normal, NULL, NULL, &rect.w, &rect.h);

    this->action = action;
}

HyperLink::~HyperLink() {}

void HyperLink::update() {
    int x,y;
    SDL_GetMouseState(&x, &y);
    if (x >= rect.x && x <= rect.x + rect.w &&
        y >= rect.y && y <= rect.y + rect.h
    ) {
        hovered = true;
    } else {
        hovered = false;
    }

    if (hovered && Application::event->mouseClickLeft()) {
        Application::event->handleClick(action);
    }
}

void HyperLink::draw() {
    if (hovered)
        TextureManager::Draw(hover, nullptr, &rect);
    else
        TextureManager::Draw(normal, nullptr, &rect);
}

void HyperLink::destroy() {
    SDL_DestroyTexture(normal);
    normal = nullptr;

    SDL_DestroyTexture(hover);
    hover = nullptr;
}
