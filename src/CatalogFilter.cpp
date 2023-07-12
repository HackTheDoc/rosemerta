#include "include/CatalogFilter.h"
#include "include/TextureManager.h"
#include "include/Application.h"

CatalogFilter::CatalogFilter(int w, int h) {
    comment = TextureManager::GenerateText("Click to change!", "comment");
    SDL_QueryTexture(comment, NULL, NULL, &commentRect.w, &commentRect.h);
    commentBorder.w = commentRect.w + 4;
    commentBorder.h = commentRect.h + 2;

    text = TextureManager::GenerateText("Filter by ", "default");
    SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);

    filters.push_back(TextureManager::GenerateText("id", "default", 512, "font"));
    filters.push_back(TextureManager::GenerateText("name", "default", 512, "font"));


    SDL_QueryTexture(filters[0], NULL, NULL, &rect.w, &rect.h);
    SDL_QueryTexture(filters[1], NULL, NULL, &rect.w, &rect.h);

    selected = 0;
    
    rect.w = w;
    rect.h = h;
    selectorRect.w = w - textRect.w;
    selectorRect.h = h;
}

CatalogFilter::~CatalogFilter() {}

void CatalogFilter::update() {
    SDL_QueryTexture(filters[selected], NULL, NULL, &filterRect.w, &filterRect.h);
    filterRect.x = selectorRect.x + (selectorRect.w - filterRect.w) / 2;
    filterRect.y = selectorRect.y + (selectorRect.h - filterRect.h) / 2 + 4;

    int x,y;
    SDL_GetMouseState(&x, &y);
    if (x >= selectorRect.x && x <= selectorRect.x + selectorRect.w &&
        y >= selectorRect.y && y <= selectorRect.y + selectorRect.h
    ) {
        commentRect.x = x;
        commentRect.y = y;
        commentBorder.x = x - 2;
        commentBorder.y = y - 2;
        hovering = true;
    } else {
        hovering = false;
    }

    if (Application::event->mouseClickLeftIn(selectorRect)) {
        selected++;

        if (selected >= (int)filters.size()) selected = 0;
    }
}

void CatalogFilter::draw() {
    TextureManager::Draw(text, nullptr, &textRect);

    TextureManager::DrawFilledRect(&selectorRect, "white");
    TextureManager::Draw(filters[selected], nullptr, &filterRect);

    TextureManager::DrawRect(&selectorRect, "border");

    if (hovering) {
        TextureManager::DrawFilledRect(&commentBorder, "background");
        TextureManager::DrawRect(&commentBorder, "black");
        TextureManager::Draw(comment, nullptr, &commentRect);
    }
}

void CatalogFilter::destroy() {
    for (auto s : filters)
        SDL_DestroyTexture(s);
    filters.clear();

    SDL_DestroyTexture(comment);
}

void CatalogFilter::place(int x, int y) {
    rect.x = x;
    rect.y = y;

    textRect.x = x;
    textRect.y = y + 4;

    selectorRect.x = x + textRect.w;
    selectorRect.y = y;
}