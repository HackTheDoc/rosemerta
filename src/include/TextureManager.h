#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* filepath);
    static SDL_Texture* GenerateText(const char* text, std::string fontName, int length = 512, std::string color = "black");
    static void Draw(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest);
    static void DrawRect(SDL_Rect* rect, std::string color = "black");
};
