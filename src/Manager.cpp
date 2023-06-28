#include "include/Manager.h"
#include "include/Window.h"

const int Manager::DEFAULT_FONT_SIZE    = 32;
const int Manager::TITLE_FONT_SIZE      = 64;
const int Manager::SUBTITLE_FONT_SIZE   = 48;
const int Manager::COMMENTARY_FONT_SIZE = 16;

EventManager* Manager::event = nullptr;
std::string Manager::database = "";

Manager::Manager() {}

Manager::~Manager() {}

void Manager::init() {
    event = new EventManager();

    // colors
    color["black"]          = {  0,   0,   0, 255};
    color["white"]          = {255, 255, 255, 255};
    color["red"]            = {255,   0,   0, 255};
    color["green"]          = {  0, 255,   0, 255};
    color["blue"]           = {  0,   0, 255, 255};
    color["pink"]           = {230,   0, 230, 255};
    color["turquoise"]      = {  0, 204, 204, 255};
    color["gray"]           = {127, 127, 127, 255};
    color["light gray"]     = {192, 192, 192, 255};
    color["background"]     = {229, 252, 245, 255}; // mint green
    color["border"]         = {117,  13,  55, 255}; // claret
    color["font"]           = { 33,   1,  36, 255}; // dark purple

    // fonts
    font["default"]         = TTF_OpenFont("./assets/fonts/Oxanium-Regular.ttf", DEFAULT_FONT_SIZE);
    font["title"]           = TTF_OpenFont("./assets/fonts/Oxanium-Bold.ttf", TITLE_FONT_SIZE);
    font["subtitle"]        = TTF_OpenFont("./assets/fonts/Oxanium-SemiBold.ttf", SUBTITLE_FONT_SIZE);
    font["comment"]         = TTF_OpenFont("./assets/fonts/Oxanium-Regular.ttf", COMMENTARY_FONT_SIZE);
}

void Manager::clear() {
    delete event;
    event = nullptr;
    
    color.clear();

    for (auto f : font) {
        TTF_CloseFont(f.second);
    }
    font.clear();
}

void Manager::setColor(std::string tag) {
    setColor(getColor(tag));
}

void Manager::setColor(SDL_Color c) {
    SDL_SetRenderDrawColor(Window::renderer, c.r, c.g, c.b, c.a);
}

SDL_Color Manager::getColor(std::string tag) {
    if (color.count(tag) <= 0) return color["black"];
    return color[tag];
}

TTF_Font* Manager::getFont(std::string tag) {
    if (font.count(tag) <= 0) return nullptr;
    return font[tag];
}
