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
    std::string s = "";
    if (day == -1) s += "00";
    else {
        std::string t = std::to_string(day);
        while (t.size() < 2) t.insert(t.begin(), '0');
        s += t;
    }

    s += "-";
    if (month == -1) s += "00";
    else {
        std::string t = std::to_string(month);
        while (t.size() < 2) t.insert(t.begin(), '0');
        s += t;
    }
    s += "-";
    if (year == -1) s += "0000";
    else {
        std::string t = std::to_string(year);
        while (t.size() < 4) t.insert(t.begin(), '0');
        s += t;
    }

    return s;
}

std::ostream& operator<<(std::ostream& stream, const Date& d) {
    if (d.day == -1) stream << "00";
    else {
        std::string t = std::to_string(d.day);
        while (t.size() < 2) t.insert(t.begin(), '0');
        stream << t;
    }

    stream << "-";
    if (d.month == -1) stream << "00";
    else {
        std::string t = std::to_string(d.month);
        while (t.size() < 2) t.insert(t.begin(), '0');
        stream << t;
    }
    stream << "-";
    if (d.year == -1) stream << "0000";
    else {
        std::string t = std::to_string(d.year);
        while (t.size() < 4) t.insert(t.begin(), '0');
        stream << t;
    }

    return stream;
}
