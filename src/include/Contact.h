#pragma once

#include <iostream>
#include <string>
#include <map>

class Contact {
public:
    enum Type {
        UNKNOWN,
        PHONE_NUMBER,
        TWITTER,
        INSTAGRAM,
        FACEBOOK,
        TELEGRAM,
        DISCORD,
        GITHUB
    };

    Contact(Contact::Type type = Contact::Type::UNKNOWN, std::string detail = "");
    ~Contact();

    void setType(Contact::Type type);
    void addDetail(std::string detail);

    Contact::Type getType();
    std::string getDetail();

    friend std::ostream& operator<<(std::ostream& stream, const Contact& c);

private:
    static const std::map<Contact::Type, std::string> TYPE_TO_STRING;
    
    Contact::Type type;
    std::string detail;
};
