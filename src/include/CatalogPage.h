#pragma once

#include <map>

#include "Page.h"
#include "CatalogItem.h"
#include "CatalogFilter.h"
#include "CatalogSelection.h"
#include "UIButton.h"

class CatalogPage : public Page {
public:
    CatalogPage();
    ~CatalogPage();

    void init() override;
    void update() override;
    void render() override;
    void destroy() override;

    static void Add(int id, const char* name, SDL_Texture* picture);
    static void Remove(int id);
    static void Clear();

private:
    static std::map<int, CatalogItem*> items;
    
    SDL_Rect gallery;

    CatalogSelection* selection;
    CatalogFilter* filter;
};
