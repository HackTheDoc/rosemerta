#pragma once

#include <string>
#include <map>

#include "UIElement.h"
#include "UILabel.h"
#include "Header.h"

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

private:
    std::map<std::string, UIElement*> elements;
};