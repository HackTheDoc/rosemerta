#include "include/DateDisplayer.h"
#include "include/TextureManager.h"

#include <string>

DateDisplayer::DateDisplayer(int d, int m, int y) {
    std::string t = "";

    if (d == -1) t += "00";
    else t += std::to_string(d);
    if (t.size() < 2) t.insert(0, "0");

    t += "/";

    if (m == -1) t += "00";
    else t += std::to_string(d);
    if (t.size() < 5) t.insert(3, "0");

    t += "/";

    if (y == -1 || y == 0) t += "0000";
    else t += std::to_string(d);

    texture = TextureManager::GenerateText(t.c_str(), "default");

    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
}

DateDisplayer::~DateDisplayer() {}

void DateDisplayer::draw() {
    TextureManager::Draw(texture, nullptr, &rect);
}

void DateDisplayer::destroy() {
    SDL_DestroyTexture(texture);
}
