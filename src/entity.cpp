#include "include/entity.h"

std::string to_string(const Contact c) {
    std::string s = c.u;

    if (!c.p.empty())
        s += " (" + c.p + ")";

    return s;
}

std::string to_string(const Contact::Type t) {
    switch (t) {
    case Contact::Type::EMAIL:
        return "email";
    case Contact::Type::PHONE_NUMBER:
        return "phone number";
    case Contact::Type::DISCORD:
        return "discord";
    case Contact::Type::INSTAGRAM:
        return "instagram";
    case Contact::Type::SNAPCHAT:
        return "snapchat";
    case Contact::Type::TWITTER:
        return "twitter";
    case Contact::Type::TELEGRAM:
        return "telegram";
    case Contact::Type::BEREAL:
        return "bereal";
    case Contact::Type::UNKNOWN:
        return "unknown";
    case Contact::Type::INVALID:
    default:
        return "";
    }
}

Contact::Type to_contact_type(const int t) {
    switch (t) {
    case 1:
        return Contact::Type::UNKNOWN;
    case 2:
        return Contact::Type::EMAIL;
    case 3:
        return Contact::Type::PHONE_NUMBER;
    case 4:
        return Contact::Type::DISCORD;
    case 5:
        return Contact::Type::INSTAGRAM;
    case 6:
        return Contact::Type::SNAPCHAT;
    case 7:
        return Contact::Type::TWITTER;
    case 8:
        return Contact::Type::TELEGRAM;
    case 9:
        return Contact::Type::BEREAL;
    default:
        return Contact::Type::INVALID;
    }
}

Contact::Type to_contact_type(const std::string& t) {
    if (t == "email")
        return Contact::Type::EMAIL;
    else if (t == "phone" || t == "phone_number" || t == "number")
        return Contact::Type::PHONE_NUMBER;
    else if (t == "discord")
        return Contact::Type::DISCORD;
    else if (t == "instagram")
        return Contact::Type::INSTAGRAM;
    else if (t == "snapchat")
        return Contact::Type::SNAPCHAT;
    else if (t == "twitter" || t == "x")
        return Contact::Type::TWITTER;
    else if (t == "telegram")
        return Contact::Type::TELEGRAM;
    else if (t == "bereal")
        return Contact::Type::BEREAL;
    else if (t == "unknown")
        return Contact::Type::UNKNOWN;
    else
        return Contact::Type::INVALID;
}

Entity::Status to_status(const int s) {
    switch (s) {
    case 0:
        return Entity::Status::ALIVE;
    case 1:
        return Entity::Status::DEAD;
    default:
        return Entity::Status::UNKNOWN;
    }
}
