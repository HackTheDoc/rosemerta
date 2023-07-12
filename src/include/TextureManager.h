#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* filepath);
    static SDL_Texture* GenerateText(const char* text, std::string fontName, int length = 512, std::string color = "font");
    static void Draw(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest);
    static void DrawRect(SDL_Rect* rect, std::string color = "border", int borderWidth = 1);
    static void DrawFilledRect(SDL_Rect* rect, std::string color = "border");
    static void DrawLine(int x1, int y1, int x2, int y2, std::string color = "border");
    static void SetViewport(SDL_Rect* rect);
};
