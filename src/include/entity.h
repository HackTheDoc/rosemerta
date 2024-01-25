#pragma once

#include <string>
#include <vector>

struct Address {
    std::string name{ "" };
    std::string detail{ "" };
};

struct Contact {
    enum Type {
        INVALID,

        UNKNOWN,

        EMAIL,
        PHONE_NUMBER,

        DISCORD,
        INSTAGRAM,
        SNAPCHAT,
        TWITTER,
        TELEGRAM,
        BEREAL
    };
    Type t{ Type::UNKNOWN };
    std::string u{ "" };
    std::string p{ "" };
};

std::string to_string(const Contact c);
std::string to_string(const Contact::Type t);

Contact::Type to_contact_type(const int t);

Contact::Type to_contact_type(const std::string& t);

struct Entity {
    enum Status {
        ALIVE,
        DEAD,
        UNKNOWN
    };

    std::string formatted_name{ "" };
    std::string firstname{ "" };
    std::string lastname{ "" };
    std::string username{ "" };
    int age{ -1 };
    std::string birthday{ "" };
    Status status{ Status::UNKNOWN };
    std::vector<Address> addresses{};
    std::vector<Contact> contacts{};
    std::vector<std::string> notes{};
};

Entity::Status to_status(const int s);
