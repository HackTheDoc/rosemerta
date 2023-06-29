#pragma once

#include <string>
#include <map>

#include "UIElement.h"
#include "Header.h"

#include "Page.h"
#include "LoginPage.h"
#include "RegisterPage.h"
#include "IdentityEditorPage.h"

class UI {
public:
    UI();
    ~UI();

    void init();
    void update();
    void render();
    void destroy();

    void add(std::string tag, UIElement* elt);
    void remove(std::string tag);

    void openPage(Page::Type p);
    void validPage();

private:
    std::map<std::string, UIElement*> elements;
    Page* currentPage;
};
