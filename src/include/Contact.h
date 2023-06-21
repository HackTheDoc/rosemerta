#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

/// @brief A class representing a contact book storing contact of one type only
class Contact {
public:
    /// @brief the different existing types of contact
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

    /// @brief Create a new contact
    /// @param type default: UNKNOWN
    /// @param detail default: ""
    Contact(Contact::Type type = Contact::Type::UNKNOWN, std::string detail = "");
    ~Contact();
    
    /// @brief add multiple details to the contact
    /// @param details a single string containing one or more detail(s)
    void addDetails(std::string details);
    /// @brief add another detail to the contact
    /// @param detail string containing the detail to store
    void addDetail(std::string detail);
    /// @brief remove one single detail stored in the contact
    /// @param index index of the detail to remove
    /// @return true if successfully removed, else false
    bool removeDetail(int index);

    /// @brief get the type of contact stored
    /// @return a contact type value
    Contact::Type getType();
    /// @brief get every details of the contact as one single string
    /// @return THE string
    std::string getDetails();

    friend std::ostream& operator<<(std::ostream& stream, const Contact& c);
    
    /// @brief converter map from string with its associated contact type
    static const std::map<std::string, Contact::Type> STRING_TO_TYPE;


private:
    /// @brief converter map from contact type to its associated string value
    static const std::map<Contact::Type, std::string> TYPE_TO_STRING;
    
    /// @brief current type of the contact
    Contact::Type type;
    /// @brief every contact registered
    std::vector<std::string> details;
};
