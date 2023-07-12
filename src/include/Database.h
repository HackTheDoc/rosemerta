#pragma once

#include <SDL2/SDL.h>
#include <sqlite3.h>
#include <vector>
#include <string>

class Database {
public:
    static void SetPath(std::string path);

    static void Create();

    static void LoadCatalog();
    static SDL_Texture* GetPicture(int id);
    static std::string GetName(int id);
    static std::vector<int> GetBirthDay(int id);
    static int GetAge(int id);
    static int GetStatus(int id);
    
private:
    static std::string path;
    static sqlite3* db;
};
