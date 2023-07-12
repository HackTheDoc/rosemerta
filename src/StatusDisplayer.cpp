#include "include/StatusDisplayer.h"
#include "include/TextureManager.h"
#include "include/Application.h"

#include <iostream>

StatusDisplayer::StatusDisplayer(int s) {
    if (s = 0)
        status = TextureManager::GenerateText("UNKNOWN", "default", 512, "gray");
    if (s = 1)
        status = TextureManager::GenerateText("ALIVE", "default", 512, "green");
    if (s = 2)
        status = TextureManager::GenerateText("DEAD", "default", 512, "red");

    SDL_QueryTexture(status, NULL, NULL, &rect.w, &rect.h);
}

StatusDisplayer::~StatusDisplayer() {}

void StatusDisplayer::draw() {
    TextureManager::Draw(status, nullptr, &rect);
}

void StatusDisplayer::destroy() {
    SDL_DestroyTexture(status);
}
