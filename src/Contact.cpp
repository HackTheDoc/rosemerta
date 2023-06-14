#include "include/Contact.h"

const std::map<Contact::Type, std::string> Contact::TYPE_TO_STRING = {
    {Contact::Type::UNKNOWN,       "Unknown"       },
    {Contact::Type::PHONE_NUMBER,  "Phone Number"  },
    {Contact::Type::TWITTER,       "Twitter"       },
    {Contact::Type::INSTAGRAM,     "Instagram"     },
    {Contact::Type::FACEBOOK,      "Facebook"      },
    {Contact::Type::TELEGRAM,      "Telegram"      },
    {Contact::Type::DISCORD,       "Discord"       }
};

Contact::Contact(Contact::Type type, std::string detail) {
    setType(type);
    addDetail(detail);
}

Contact::~Contact() {}

void Contact::setType(Contact::Type type) {
    this->type = type;
}

void Contact::addDetail(std::string detail) {
    // TODO: add a patern verification with the type of contact
    this->detail = detail;
}

Contact::Type Contact::getType() {
    return type;
}

std::string Contact::getDetail() {
    return detail;
}

std::ostream& operator<<(std::ostream& stream, const Contact& c) {
    stream << c.TYPE_TO_STRING.at(c.type) << ": " << c.detail;
    return stream;
}
