#include "include/InputField.h"
#include "include/Manager.h"
#include "include/Window.h"

SDL_Rect InputField::cursor = {0,0,3,0};

InputField::InputField(const char* placeholder) {
    this->placeholder = TextureManager::GenerateText(
        placeholder,
        "default",
        512,
        "light gray"
    );
    SDL_QueryTexture(this->placeholder, NULL, NULL, &field.w, &field.h);

    text = nullptr;

    rect.w = field.w + 4;
    rect.h = field.h + 4;

    actived = false;

    textRect = {0,0,0,0};
}

InputField::InputField(const char* placeholder, int w, int h) {
    rect.w = w;
    rect.h = h;

    this->placeholder = TextureManager::GenerateText(
        placeholder,
        "default",
        w,
        "light gray"
    );
    field = {0, 0, w-4, h-4};

    text = nullptr;

    actived = false;

    textRect = {0,0,0,0};
}

InputField::~InputField() {}

void InputField::update() {
    if (input.empty()) {
        SDL_QueryTexture(placeholder, NULL, NULL, &textRect.w, &textRect.h);
        textRect.x = field.x;
        textRect.y = field.y + (field.h - textRect.h) / 2 + 4;
    }

    if (Manager::event->mouseClickLeftIn(rect))
        actived = true;
    else if (Manager::event->mouseClickLeft() || Manager::event->mouseClickRight())
        actived = false;
    
    if (!actived) return;

    if (Manager::event->e.type == SDL_TEXTINPUT) {
        input += Manager::event->e.text.text;
    }
    else if (Manager::event->e.type == SDL_KEYDOWN &&
            Manager::event->e.key.keysym.sym == SDLK_BACKSPACE &&
            !input.empty()
    ) {
        input.pop_back();   
    }

    if (!input.empty()) {
        text = TextureManager::GenerateText(
            input.c_str(),
            "default",
            1024
        );
        SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);
        textRect.y = (field.h - textRect.h) / 2 + 4;
        textRect.x = 0;
        if (textRect.w >= field.w) textRect.x -= textRect.w - field.w;
    }
}

void InputField::draw() {
    TextureManager::DrawFilledRect(&rect, "white");
    TextureManager::DrawRect(&rect, "black");
    if (input.empty()) {
        TextureManager::Draw(placeholder, nullptr, &textRect);
        if (actived) {
            TextureManager::DrawLine(
                field.x,
                field.y,
                field.x,
                field.y + field.h - 2,
                "red"
            );
        }
        return;
    }

    SDL_RenderSetViewport(Window::renderer, &field);
    TextureManager::Draw(text, nullptr, &textRect);
    if (actived) {
        TextureManager::DrawLine(
            textRect.x + textRect.w,
            textRect.y,
            textRect.x + textRect.w,
            textRect.y + textRect.h - 2,
            "red"
        );
    }
    SDL_RenderSetViewport(Window::renderer, NULL);
}

void InputField::destroy() {
    SDL_DestroyTexture(placeholder);
    placeholder = nullptr;

    if (text == nullptr) return;
    SDL_DestroyTexture(text);
    text = nullptr;
}

void InputField::place(int x, int y) {
    UIElement::place(x,y);
    field.x = x+2;
    field.y = y + (rect.h - field.h) / 2;
}

bool InputField::selected() {
    return actived;
}

void InputField::select() {
    actived = true;
}

void InputField::unselect() {
    actived = false;
}
