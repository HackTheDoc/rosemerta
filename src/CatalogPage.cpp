#include "include/CatalogPage.h"
#include "include/Application.h"
#include "include/TextureManager.h"
#include "include/Database.h"

#include <iostream>

std::map<int, CatalogItem*> CatalogPage::items = {};

CatalogPage::CatalogPage() {
    rect = {0, 64, Application::window->screen.w, Application::window->screen.h - 64};

    selection = new CatalogSelection(192, rect.h);
    selection->place(0, 64);

    gallery = {selection->width(), 64, rect.w - selection->width(), rect.h};
}

CatalogPage::~CatalogPage() {}

void CatalogPage::init() {
    filter = new CatalogFilter(224, 42);
    filter->place(
        gallery.x + 16,
        gallery.y + 16
    );

    Database::LoadCatalog();
}

void CatalogPage::update() {
    filter->update();

    int i = 0;
    int j = 0;
    for (auto item : items) {
        item.second->update();

        item.second->place(
            gallery.x + 16 + i * (16+item.second->width()),
            filter->y() + filter->height() + 16 + j * (16 + item.second->height())
        );

        i++;
        if (i > 3) {
            i = 1;
            j++;
            if (j > 3) break;
        }
    }
    
    selection->update();
}

void CatalogPage::render() {
    selection->draw();

    filter->draw();

    for (auto i : items) {
        i.second->draw();
    }
}

void CatalogPage::destroy() {
    Clear();
    
    selection->destroy();

    filter->destroy();
}

void CatalogPage::Add(int id, const char* name, SDL_Texture* picture) {
    CatalogItem* i = new CatalogItem(id, name, picture);
    items[id] = i;
}

void CatalogPage::Remove(int id) {
    if (items.count(id) == 0) return;
    items[id]->destroy();
    items.erase(id);
}

void CatalogPage::Clear() {
    for (auto i : items) {
        i.second->destroy();
    }
    items.clear();
}
