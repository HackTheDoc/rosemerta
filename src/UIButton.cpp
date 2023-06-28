#include "include/UIButton.h"
#include "include/Manager.h"
#include "include/Window.h"

UIButton::UIButton(std::string text, std::string action) {
    this->text = TextureManager::GenerateText(text.c_str(), "default");
    textRect.x = textRect.y = 2;
    SDL_QueryTexture(this->text, NULL, NULL, &textRect.w, &textRect.h);

    rect.x = rect.y = 0;
    rect.w = textRect.w + 4;
    rect.h = textRect.h + 4;
}

UIButton::UIButton(std::string text, std::string action, int w, int h) {
    this->text = TextureManager::GenerateText(text.c_str(), "default", w);
    SDL_QueryTexture(this->text, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = (w - textRect.w) / 2;
    textRect.y = (h - textRect.h) / 2;

    rect.x = rect.y = 0;
    rect.w = w;
    rect.h = h;
}

UIButton::~UIButton() {}

void UIButton::update() {
    int x,y;
    SDL_GetMouseState(&x, &y);
    if (x >= rect.x && x <= rect.x + rect.w &&
        y >= rect.y && y <= rect.y + rect.h
    ) {
        hovered = true;
    } else {
        hovered = false;
    }

    if (hovered && Manager::event->mouseClickLeft()) {
        Manager::event->handleClick(action);
    }
}

void UIButton::draw() {
    if (hovered)
        TextureManager::DrawFilledRect(&rect, "gray");
    else
        TextureManager::DrawFilledRect(&rect, "light gray");
    SDL_RenderSetViewport(Window::renderer, &rect);
    TextureManager::Draw(text, nullptr, &textRect);
    SDL_RenderSetViewport(Window::renderer, NULL);
    TextureManager::DrawRect(&rect, "black");
}

void UIButton::destroy() {
    SDL_DestroyTexture(text);
    text = nullptr;
}
