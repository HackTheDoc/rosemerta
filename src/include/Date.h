#pragma once

#include <iostream>
#include <string>

class Date {
public:
    int year;
    int month;
    int day;

    Date(int d = -1, int m = -1, int y = -1);
    ~Date();

    void setDate(int d, int m, int y);
    std::string get();

    friend std::ostream& operator<<(std::ostream& stream, const Date& d);
};
