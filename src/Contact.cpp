#include "include/Contact.h"

#include <sstream>

const std::map<Contact::Type, std::string> Contact::TYPE_TO_STRING = {
    {Contact::Type::UNKNOWN,       "Unknown     "},
    {Contact::Type::PHONE_NUMBER,  "Phone Number"},
    {Contact::Type::TWITTER,       "Twitter     "},
    {Contact::Type::INSTAGRAM,     "Instagram   "},
    {Contact::Type::FACEBOOK,      "Facebook    "},
    {Contact::Type::TELEGRAM,      "Telegram    "},
    {Contact::Type::DISCORD,       "Discord     "}
};

const std::map<std::string, Contact::Type> Contact::STRING_TO_TYPE = {
    {"unknown"      , Contact::Type::UNKNOWN        },
    {"phone-number" , Contact::Type::PHONE_NUMBER   },
    {"twitter"      , Contact::Type::TWITTER        },
    {"instagram"    , Contact::Type::INSTAGRAM      },
    {"facebook"     , Contact::Type::FACEBOOK       },
    {"telegram"     , Contact::Type::TELEGRAM       },
    {"discord"      , Contact::Type::DISCORD        }
};

Contact::Contact(Contact::Type type, std::string detail) {
    setType(type);
    addDetails(detail);
}

Contact::~Contact() {}

void Contact::setType(Contact::Type type) {
    this->type = type;
}

void Contact::addDetails(std::string details) {
    std::stringstream ss(details);
    std::string s;
    while (getline(ss, s, '-')) {
        addDetail(s);
    }
}

void Contact::addDetail(std::string detail) {
    // TODO: add a patern verification with the type of contact
    details.push_back(detail);
}

bool Contact::removeDetail(int index) {
    if (index > (int)details.size()) return false;

    details.erase(details.begin() + (index-1));
    return (int)details.size() == 0;
}

Contact::Type Contact::getType() {
    return type;
}

std::string Contact::getDetails() {
    std::string s = details.at(0);
    for (int i = 1; i < (int)details.size(); i++)
        s += "-" + details.at(i);
    return s;
}

std::ostream& operator<<(std::ostream& stream, const Contact& c) {
    stream << c.TYPE_TO_STRING.at(c.type) << ": ";
    stream << "\033[33m";
    stream << c.details.at(0);
    for (int i = 1; i < (int)c.details.size(); i++) {
        stream << std::endl; 
        std::string span = "    ";
        for (int n = 0; n < (int)c.TYPE_TO_STRING.at(c.type).size(); n++)
            span += " ";
        stream << span << c.details.at(i);
    }
    stream << "\033[0m";
    return stream;
}
