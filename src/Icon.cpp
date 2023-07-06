#include "include/Icon.h"
#include "include/TextureManager.h"
#include "include/Application.h"

Icon::Icon(std::string iconpath, std::string text, std::string action, int x, int y) {
    icon = TextureManager::LoadTexture(iconpath.c_str());
    rect.x = x;
    rect.y = y;
    rect.w = rect.h = 48;

    comment = TextureManager::GenerateText(text.c_str(), "comment");
    SDL_QueryTexture(comment, NULL, NULL, &commentRect.w, &commentRect.h);
    commentBorder.w = commentRect.w + 4;
    commentBorder.h = commentRect.h + 2;

    this->action = action;
}

Icon::~Icon() {}

void Icon::update() {
    int x,y;
    SDL_GetMouseState(&x, &y);
    if (x >= rect.x && x <= rect.x + rect.w &&
        y >= rect.y && y <= rect.y + rect.h
    ) {
        commentRect.x = x;
        commentRect.y = y;
        commentBorder.x = x - 2;
        commentBorder.y = y - 2;
        hovering = true;
    } else {
        hovering = false;
    }

    if (hovering && Application::event->mouseClickLeft()) {
        Application::event->handleClick(action);
    }
}

void Icon::draw() {
    TextureManager::Draw(icon, nullptr, &rect);
    if (hovering) {
        TextureManager::DrawFilledRect(&commentBorder, "background");
        TextureManager::DrawRect(&commentBorder, "black");
        TextureManager::Draw(comment, nullptr, &commentRect);
    }
}

void Icon::destroy() {
    SDL_DestroyTexture(icon);
    icon = nullptr;

    SDL_DestroyTexture(comment);
    comment = nullptr;
}

