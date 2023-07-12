#include "include/TextureManager.h"
#include "include/Application.h"

#include <vector>

SDL_Texture* TextureManager::LoadTexture(const char* filepath) {
    SDL_Surface* s = IMG_Load(filepath);

    SDL_Texture* t = SDL_CreateTextureFromSurface(Application::window->renderer, s);

    SDL_FreeSurface(s);

    return t;
}

SDL_Texture* TextureManager::GenerateText(const char* text, std::string fontName, int length, std::string color) {
    TTF_Font* f = Application::window->getFont(fontName);
    if (f == nullptr) return nullptr;

    SDL_Color c = Application::window->getColor(color);

    SDL_Surface* s = TTF_RenderUTF8_Blended_Wrapped(f, text, c, length);

    SDL_Texture* t = SDL_CreateTextureFromSurface(Application::window->renderer, s);

    SDL_FreeSurface(s);

    return t;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest) {
    if (texture == nullptr) return;

    SDL_RenderCopy(Application::window->renderer, texture, src, dest);
}

void TextureManager::DrawRect(SDL_Rect* rect, std::string color, int borderWidth) {
    std::vector<SDL_Rect> buffer;
    for (int i = 1; i < borderWidth; i++) {
        SDL_Rect r = {rect->x-i, rect->y-i, rect->w+(2*i), rect->h+(2*i)};
        buffer.push_back(r);
    }

    SDL_Color t;
    SDL_GetRenderDrawColor(Application::window->renderer, &t.r, &t.g, &t.b, &t.a);
    
    Application::window->setColor(color);

    SDL_RenderDrawRect(Application::window->renderer, rect);
    for (auto r : buffer)
        SDL_RenderDrawRect(Application::window->renderer, &r);

    Application::window->setColor(t);

    buffer.clear();
}

void TextureManager::DrawFilledRect(SDL_Rect* rect, std::string color) {
    SDL_Color t;
    SDL_GetRenderDrawColor(Application::window->renderer, &t.r, &t.g, &t.b, &t.a);
    
    Application::window->setColor(color);

    SDL_RenderFillRect(Application::window->renderer, rect);

    Application::window->setColor(t);
}

void TextureManager::DrawLine(int x1, int y1, int x2, int y2, std::string color) {
    SDL_Color t;
    SDL_GetRenderDrawColor(Application::window->renderer, &t.r, &t.g, &t.b, &t.a);
    
    Application::window->setColor(color);

    SDL_RenderDrawLine(Application::window->renderer, x1, y1, x2, y2);
    
    Application::window->setColor(t);
}

void TextureManager::SetViewport(SDL_Rect* rect) {
    SDL_RenderSetViewport(Application::window->renderer, rect);
}
