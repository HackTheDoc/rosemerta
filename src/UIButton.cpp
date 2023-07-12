#include "include/UIButton.h"
#include "include/TextureManager.h"
#include "include/Application.h"

UIButton::UIButton(std::string text, std::string action) {
    this->text = TextureManager::GenerateText(text.c_str(), "default");
    textRect.x = textRect.y = 2;
    SDL_QueryTexture(this->text, NULL, NULL, &textRect.w, &textRect.h);

    rect.x = rect.y = 0;
    rect.w = textRect.w + 4;
    rect.h = textRect.h + 4;

    this->action = action;
}

UIButton::UIButton(std::string text, std::string action, int w, int h) {
    this->text = TextureManager::GenerateText(text.c_str(), "default", w);
    SDL_QueryTexture(this->text, NULL, NULL, &textRect.w, &textRect.h);

    rect.x = rect.y = 0;
    rect.w = w;
    rect.h = h;

    this->action = action;
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

    if (hovered && Application::event->mouseClickLeft()) {
        use();
    }
}

void UIButton::draw() {
    if (hovered)
        TextureManager::DrawFilledRect(&rect, "gray");
    else
        TextureManager::DrawFilledRect(&rect, "light gray");
    TextureManager::Draw(text, nullptr, &textRect);
    TextureManager::DrawRect(&rect, "black");
}

void UIButton::destroy() {
    SDL_DestroyTexture(text);
    text = nullptr;
}

void UIButton::use() {
    Application::event->handleClick(action);
}

void UIButton::place(int x, int y) {
    rect.x = x;
    rect.y = y;
    textRect.x = x + (rect.w - textRect.w) / 2;
    textRect.y = y + (rect.h - textRect.h) / 2 + 4;
}
