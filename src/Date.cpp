#include "include/Date.h"

Date::Date(int d, int m, int y) {
    setDate(d,m,y);
}

Date::~Date() {}

void Date::setDate(int d, int m, int y) {
    day = d;
    month = m;
    year = y;

    if (day > 31 || day < 1) day = -1;
    if (month > 12 || month < 1) month = -1;
}

std::string Date::get() {
    return std::to_string(day) + "-" + std::to_string(month) + "-" + std::to_string(year);
}

std::ostream& operator<<(std::ostream& stream, const Date& d) {
    stream << std::to_string(d.day)     << "-";
    stream << std::to_string(d.month)   << "-" ;
    stream << std::to_string(d.year);
    return stream;
}
