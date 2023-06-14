#pragma once

#include <iostream>
#include <string>
#include <map>

class Location {
public:
    enum Type {
        UNKNOWN,
        HOME,
        WORKPLACE
    };

    Location(Location::Type t = Location::Type::UNKNOWN);
    Location(std::string address, Location::Type t = Location::Type::UNKNOWN);
    Location(std::string number, std::string street, std::string city, std::string department, std::string country, Location::Type t = Location::Type::UNKNOWN);
    ~Location();

    void setType(Location::Type t);
    Location::Type getType();

    void setAddress(std::string address);
    std::string getAddress();

    void setNumber(std::string n);
    std::string getNumber();

    void setStreet(std::string s);
    std::string getStreet();

    void setCity(std::string c);
    std::string getCity();

    void setDepartment(std::string d);
    std::string getDepartment();

    void setCountry(std::string c);
    std::string getCountry();

    friend std::ostream& operator<<(std::ostream& stream, const Location& l);

private:
    static const std::map<Location::Type, std::string> TYPE_TO_STRING;
    
    Location::Type type;

    std::string number;
    std::string street;
    std::string city;
    std::string department;
    std::string country;
};
