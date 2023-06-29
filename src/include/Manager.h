#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <map>

#include "TextureManager.h"
#include "EventManager.h"

class Window;

class Manager {
public:
    static const int DEFAULT_FONT_SIZE;
    static const int TITLE_FONT_SIZE;
    static const int SUBTITLE_FONT_SIZE;
    static const int COMMENTARY_FONT_SIZE;
    
    static EventManager* event;
    static std::string database;
    static std::pair<std::string, std::string> user;
    static int selectedIdentity;

    Manager();
    ~Manager();

    void init();
    void clear();

    void setColor(std::string tag);
    void setColor(SDL_Color c);
    SDL_Color getColor(std::string tag);

    TTF_Font* getFont(std::string tag);

    static void Encrypt(const std::string& path, const std::string& key);
    static void Decrypt(const std::string& path, const std::string& key);

private:
    std::map<std::string, SDL_Color> color;
    std::map<std::string, TTF_Font*> font;

    static void EncryptFile(const std::string& path, const std::string& key);
    static void DecryptFile(const std::string& path, const std::string& key);
};
