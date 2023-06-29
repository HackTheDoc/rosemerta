#pragma once

#include <vector>
#include <string>

#include "UIElement.h"
#include "InputField.h"

class DateInput : public UIElement {
public:    
    DateInput(int w, int h);
    ~DateInput();

    void update() override;
    void draw() override;
    void destroy() override;

    void place(int x, int y);
    void clear();
    std::string getDate();

    bool selectedDay();
    void selectDay();
    void unselectDay();
    bool selectedMonth();
    void selectMonth();
    void unselectMonth();
    bool selectedYear();
    void selectYear();
    void unselectYear();

private:
    InputField* day;
    InputField* month;
    InputField* year;

    SDL_Texture* separator;
    SDL_Rect leftSeparator, rightSeparator;
};
