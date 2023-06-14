#include "include/Location.h"

#include <sstream>
#include <vector>

const std::map<Location::Type, std::string> Location::TYPE_TO_STRING = {
    {Location::Type::UNKNOWN,   "Unknown"   },
    {Location::Type::HOME,      "Home"      },
    {Location::Type::WORKPLACE, "Workplace" }
};

Location::Location(Location::Type t) {
    setType(t);
    number = street = city = department = country = "unknown";
}

Location::Location(std::string address, Location::Type t) {
    setType(t);
    setAddress(address);
}

Location::Location(std::string number, std::string street, std::string city, std::string department, std::string country, Location::Type t) {
    setType(t);
    setNumber(number);
    setStreet(street);
    setCity(city);
    setDepartment(department);
    setCountry(country);
}

Location::~Location() {}

void Location::setType(Location::Type t) {
    type = t;
}

Location::Type Location::getType() {
    return type;
}

void Location::setAddress(std::string address) {
    std::vector<std::string> v;
    std::string s;
    std::stringstream ss(address);
    while (getline(ss, s, ' ')) {
        v.push_back(s);
    }
    // to continue
}

std::string Location::getAddress() {
    return number + " " + street + " " + city + " " + department + " " + country;
}

void Location::setNumber(std::string n) {
    number = n;
}

std::string Location::getNumber() {
    return number;
}

void Location::setStreet(std::string s) {
    street = s;
}

std::string Location::getStreet() {
    return street;
}

void Location::setCity(std::string c) {
    city = c;
}

std::string Location::getCity() {
    return city;
}

void Location::setDepartment(std::string d) {
    department = d;
}

std::string Location::getDepartment() {
    return department;
}

void Location::setCountry(std::string c) {
    country = c;
}

std::string Location::getCountry() {
    return country;
}

std::ostream& operator<<(std::ostream& stream, const Location& l) {
    // TODO
    
    return stream;
}
