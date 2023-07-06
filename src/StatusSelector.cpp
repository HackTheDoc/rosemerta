#include "include/StatusSelector.h"
#include "include/TextureManager.h"
#include "include/Application.h"

#include <iostream>

StatusSelector::StatusSelector(int w, int h) {
    comment = TextureManager::GenerateText("Click to change!", "comment");
    SDL_QueryTexture(comment, NULL, NULL, &commentRect.w, &commentRect.h);
    commentBorder.w = commentRect.w + 4;
    commentBorder.h = commentRect.h + 2;

    status.push_back(TextureManager::GenerateText("UNKNOWN", "default", 512, "gray"));
    status.push_back(TextureManager::GenerateText("ALIVE", "default", 512, "green"));
    status.push_back(TextureManager::GenerateText("DEAD", "default", 512, "red"));

    selected = 0;
    
    rect.w = w;
    rect.h = h;
}

StatusSelector::~StatusSelector() {}

void StatusSelector::update() {
    SDL_QueryTexture(status[selected], NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = rect.x + (rect.w - textRect.w) / 2;
    textRect.y = rect.y + (rect.h - textRect.h) / 2 + 4;

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

    if (Application::event->mouseClickLeftIn(rect)) {
        selected++;

        if (selected > 2) selected = 0;
    }
}

void StatusSelector::draw() {
    TextureManager::DrawFilledRect(&rect, "white");

    TextureManager::Draw(status[selected], nullptr, &textRect);

    TextureManager::DrawRect(&rect, "border");

    if (hovering) {
        TextureManager::DrawFilledRect(&commentBorder, "background");
        TextureManager::DrawRect(&commentBorder, "black");
        TextureManager::Draw(comment, nullptr, &commentRect);
    }
}

void StatusSelector::destroy() {
    for (auto s : status)
        SDL_DestroyTexture(s);
    status.clear();

    SDL_DestroyTexture(comment);
}
