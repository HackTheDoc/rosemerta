#include "include/CatalogItem.h"
#include "include/Application.h"
#include "include/TextureManager.h"

int CatalogItem::h = 160;

CatalogItem::CatalogItem(int id, const char* text, SDL_Texture* pic) {
    rect = {0, 0, 128, 160};

    this->id = id;

    picture = pic;
    picRect = {16, 16, 96, 96};

    int w;
    int s = TTF_SizeText(Application::window->getFont("default"), text, &w, &h);
    name = TextureManager::GenerateText(text, "default", std::min(s, rect.w-16), "font");
    SDL_QueryTexture(name, NULL, NULL, &nameRect.w, &nameRect.h);
    nameRect.x = (rect.w - nameRect.w) / 2;

    h = std::max(h, picRect.y + picRect.h + nameRect.h + 16);
}

CatalogItem::~CatalogItem() {}

void CatalogItem::update() {
    rect.h = h;
    nameRect.y = (h - nameRect.h + picRect.y + picRect.h) / 2;

    if (Application::event->mouseClickLeftIn(rect))
        Application::selectedIdentity = id;
}

void CatalogItem::draw() {
    TextureManager::DrawFilledRect(&rect, "white");

    TextureManager::SetViewport(&rect);

    //TextureManager::Draw(picture, nullptr, &picRect);
    TextureManager::DrawFilledRect(&picRect, "pink");
    TextureManager::DrawRect(&picRect);

    TextureManager::Draw(name, nullptr, &nameRect);

    TextureManager::SetViewport(nullptr);

    if (Application::selectedIdentity == id)
        TextureManager::DrawRect(&rect, "border", 2);
    else
        TextureManager::DrawRect(&rect);
}

void CatalogItem::destroy() {
    SDL_DestroyTexture(picture);
    picture = nullptr;

    SDL_DestroyTexture(name);
    name = nullptr;
}
