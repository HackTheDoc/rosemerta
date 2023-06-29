#include "include/DateInput.h"
#include "include/Manager.h"

#include <regex>
#include <iostream>

DateInput::DateInput(int w, int h) {
    separator = TextureManager::GenerateText("/", "big", 512, "black");
    SDL_QueryTexture(separator, NULL, NULL, &leftSeparator.w, &leftSeparator.h);
    rightSeparator.w = leftSeparator.w;
    rightSeparator.h = leftSeparator.h;

    day = new InputField("DD", 48, 48);
    month = new InputField("MM", 60, 48);
    year = new InputField("YYYY", 80, 48);

    rect = {0,0,w,h};
}

DateInput::~DateInput() {}

void DateInput::update() {
    day->update();
    month->update();
    year->update();
}

void DateInput::draw() {
    day->draw();
    month->draw();
    year->draw();

    TextureManager::Draw(separator, nullptr, &leftSeparator);
    TextureManager::Draw(separator, nullptr, &rightSeparator);
}

void DateInput::destroy() {
    day->destroy();
    month->destroy();
    year->destroy();

    SDL_DestroyTexture(separator);
    separator = nullptr;
}

void DateInput::place(int x, int y) {
    rect.x = x;
    rect.y = y;

    year->place(
        x + rect.w - year->width(),
        y
    );
    rightSeparator.x = year->x() - rightSeparator.w - 2;
    rightSeparator.y = y + 4;
    month->place(
        rightSeparator.x - month->width() - 2,
        y
    );
    leftSeparator.x = month->x() - leftSeparator.w - 2;
    leftSeparator.y = y + 2;
    day->place(
        leftSeparator.x - day->width() - 2,
        y
    );
}

void DateInput::clear() {
    day->input = "";
    month->input = "";
    year->input = "";
}

std::string DateInput::getDate() {
    std::regex patternDM("[0-9]{2}");
    std::regex patternY("[0-9]{4}");

    if (!day->input.empty() && !std::regex_search(day->input, patternDM)) {
        std::cout << "incorrect date syntax" << std::endl;
        return "00/00/0000";
    }
    
    if (!month->input.empty() && !std::regex_search(month->input, patternDM)) {
        std::cout << "incorrect date syntax" << std::endl;
        return "00/00/0000";
    }

    if (!year->input.empty() && !std::regex_search(year->input, patternY)) {
        std::cout << "incorrect date syntax" << std::endl;
    }

    std::string date = "";

    if (day->input.empty()) date += "00";
    else date += day->input;
    date += "/";
    if (month->input.empty()) date += "00";
    else date += month->input;
    date += "/";
    if (year->input.empty()) date += "0000";
    else date += year->input;

    return date;
}

bool DateInput::selectedDay() {
    return day->selected();
}

void DateInput::selectDay() {
    day->select();
}

void DateInput::unselectDay() {
    day->unselect();
}

bool DateInput::selectedMonth() {
    return month->selected();
}

void DateInput::selectMonth() {
    month->select();
}

void DateInput::unselectMonth() {
    month->unselect();
}

bool DateInput::selectedYear() {
    return year->selected();
}

void DateInput::selectYear() {
    year->select();
}

void DateInput::unselectYear() {
    year->unselect();
}
