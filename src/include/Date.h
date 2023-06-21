#pragma once

#include <iostream>
#include <string>

class Date {
public:
    int year; 
    int month; 
    int day;

    /// @brief create a new date
    /// @param d day
    /// @param m month
    /// @param y year
    Date(int d = -1, int m = -1, int y = -1);
    ~Date();

    /// @brief set the date value (default value being 0)
    /// @param d day between 0 and 31
    /// @param m month between 0 and 12
    /// @param y year
    void setDate(int d, int m, int y);
    /// @brief get the date as a string
    /// @return a string
    std::string get();

    friend std::ostream& operator<<(std::ostream& stream, const Date& d);
};
