#pragma once

#include <iostream>
#include <string>
#include <vector>
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
    void addDetails(std::string details);
    void addDetail(std::string detail);
    bool removeDetail(int index);

    Contact::Type getType();
    std::string getDetails();

    friend std::ostream& operator<<(std::ostream& stream, const Contact& c);
    
    static const std::map<std::string, Contact::Type> STRING_TO_TYPE;


private:
    static const std::map<Contact::Type, std::string> TYPE_TO_STRING;
    
    Contact::Type type;
    std::vector<std::string> details;
};
