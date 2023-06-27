#include "include/UILabel.h"
#include "include/Manager.h"

UILabel::UILabel(std::string text, std::string type) {
    texture = nullptr;
    this->text = text;
    this->type = type;
    rect.w = 512;
    setColor("black");
}

UILabel::UILabel(std::string text, int w, std::string type) {
    texture = nullptr;
    this->text = text;
    this->type = type;
    rect.w = w;
    setColor("black");
}

void UILabel::setColor(std::string color) {
    if (texture != nullptr) destroy();

    texture = TextureManager::GenerateText(
        text.c_str(),
        type,
        rect.w,
        color
    );
    rect.x = rect.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
}

void UILabel::draw() {
    TextureManager::Draw(texture, nullptr, &rect);
}

void UILabel::destroy() {
    SDL_DestroyTexture(texture);
}
