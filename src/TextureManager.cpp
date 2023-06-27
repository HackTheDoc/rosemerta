#include "include/TextureManager.h"
#include "include/Window.h"
#include "include/Manager.h"

SDL_Texture* TextureManager::LoadTexture(const char* filepath) {
    SDL_Surface* s = IMG_Load(filepath);

    SDL_Texture* t = SDL_CreateTextureFromSurface(Window::renderer, s);

    SDL_FreeSurface(s);

    return t;
}

SDL_Texture* TextureManager::GenerateText(const char* text, std::string fontName, int length, std::string color) {
    TTF_Font* f = Window::manager->getFont(fontName);
    if (f == nullptr) return nullptr;

    SDL_Color c = Window::manager->getColor(color);

    SDL_Surface* s = TTF_RenderText_Blended_Wrapped(f, text, c, length);

    SDL_Texture* t = SDL_CreateTextureFromSurface(Window::renderer, s);

    SDL_FreeSurface(s);

    return t;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest) {
    if (texture == nullptr) return;

    SDL_RenderCopy(Window::renderer, texture, src, dest);
}

void TextureManager::DrawRect(SDL_Rect* rect, std::string color) {
    SDL_Color t;
    SDL_GetRenderDrawColor(Window::renderer, &t.r, &t.g, &t.b, &t.a);
    
    Window::manager->setColor(color);

    SDL_RenderDrawRect(Window::renderer, rect);

    Window::manager->setColor(t);
}

void TextureManager::DrawFilledRect(SDL_Rect* rect, std::string color) {
    SDL_Color t;
    SDL_GetRenderDrawColor(Window::renderer, &t.r, &t.g, &t.b, &t.a);
    
    Window::manager->setColor(color);

    SDL_RenderFillRect(Window::renderer, rect);

    Window::manager->setColor(t);
}

void TextureManager::DrawLine(int x1, int y1, int x2, int y2, std::string color) {
    SDL_Color t;
    SDL_GetRenderDrawColor(Window::renderer, &t.r, &t.g, &t.b, &t.a);
    
    Window::manager->setColor(color);

    SDL_RenderDrawLine(Window::renderer, x1, y1, x2, y2);
    
    Window::manager->setColor(t);
}
