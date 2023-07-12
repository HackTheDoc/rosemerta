#include "include/CatalogSelection.h"
#include "include/Application.h"
#include "include/TextureManager.h"
#include "include/Database.h"

#include <vector>

CatalogSelection::CatalogSelection(int w, int h) {
    rect = {0, 0, w, h};

    picture = nullptr;
    picRect = {8, 8, w - 16, w - 16};

    status = nullptr;
    name = nullptr;
    age = nullptr;
    birthday = nullptr;

    inspectButton = new UIButton("INSPECT", "identity", w-16, 48);

    id = -1;
}

CatalogSelection::~CatalogSelection() {}

void CatalogSelection::update() {
    if (Application::selectedIdentity == -1) return;

    if (Application::selectedIdentity != id) {
        id = Application::selectedIdentity;

        if (picture) SDL_DestroyTexture(picture);
        picture = Database::GetPicture(id);

        if (status) status->destroy();
        status = new StatusDisplayer(Database::GetStatus(id));
        status->place(
            (rect.w - status->width()) /2,
            picRect.y + picRect.h + 12
        );

        if (name) name->destroy();
        name = new UILabel(Database::GetName(id), rect.w - 16);

        if (age) age->destroy();
        age = new UILabel(std::to_string(Database::GetAge(id))+" Yo", rect.w - 16);

        if (birthday) birthday->destroy();
        std::vector<int> t = Database::GetBirthDay(id);
        birthday = new DateDisplayer(t[0], t[1], t[2]);

        age->place(
            (rect.w - age->width()) /2,
            (inspectButton->y() - rect.y + status->y() + status->height() - age->height()) / 2
        );
        name->place(
            (rect.w - name->width()) /2,
            age->y() - name->height() - 8
        );
        birthday->place(
            (rect.w - birthday->width()) / 2,
            age->y() + age->height() + 8
        );
    }

    inspectButton->update();
}

void CatalogSelection::draw() {
    TextureManager::DrawFilledRect(&rect, "white");
    TextureManager::DrawLine(
        rect.x + rect.w - 1,
        rect.y,
        rect.x + rect.w - 1,
        rect.y + rect.h
    );
    TextureManager::DrawLine(
        rect.x + rect.w,
        rect.y,
        rect.x + rect.w,
        rect.y + rect.h
    );
    TextureManager::DrawLine(
        rect.x + rect.w + 1,
        rect.y,
        rect.x + rect.w + 1,
        rect.y + rect.h
    );

    if (Application::selectedIdentity == -1) return;

    TextureManager::SetViewport(&rect);

    //TextureManager::Draw(picture, nullptr, &picRect);
    TextureManager::DrawFilledRect(&picRect, "pink");
    TextureManager::DrawRect(&picRect);

    status->draw();
    name->draw();
    age->draw();
    birthday->draw();

    TextureManager::SetViewport(nullptr);

    inspectButton->draw();
}

void CatalogSelection::destroy() {
    SDL_DestroyTexture(picture);
    picture = nullptr;

    status->destroy();
    name->destroy();
    age->destroy();
    birthday->destroy();

    inspectButton->destroy();
}

void CatalogSelection::place(int x, int y) {
    rect.x = x;
    rect.y = y;

    inspectButton->place(
        (rect.w - inspectButton->width()) / 2,
        rect.h - inspectButton->height() - 8 + y
    );
}
