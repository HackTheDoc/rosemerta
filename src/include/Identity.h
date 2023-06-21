#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <map>

#include "Contact.h"
#include "Date.h"

/// @brief A class representating the storing of an identity data
class Identity {
public:
    /// @brief wether on is living or not
    enum Status {
        UNKNOWN,
        ALIVE,
        DEAD
    };

    /// @brief create a new UNKNOWN like identity
    Identity();
    ~Identity();

    /// @brief clear the identity from all of its external attributes
    void clear();

    /// @brief set the ID of the identity (1 time use only)
    /// @param id 
    void setID(int id);
    /// @brief accessor to this identity ID
    /// @return the unique ID
    int getID();

    /// @brief set the identity's username
    /// @param username the new username
    void setUsername(std::string username);
    /// @brief get the identity's username
    /// @return the username ?
    std::string getUsername();

    /// @brief set the identity's name
    /// @param name the new name
    void setName(std::string name);
    /// @brief get the identity's name
    /// @return the name ?
    std::string getName();

    /// @brief set the identity's lastname
    /// @param lastname the new lastname
    void setLastname(std::string lastname);
    /// @brief get the identity's lastname
    /// @return the lastname ?
    std::string getLastname();

    /// @brief set the identity's age
    /// @param age the new age
    void setAge(int age);
    /// @brief get the identity's age
    /// @return the age ?
    int getAge();

    /// @brief set the identity's birthday
    /// @param birthday the new birthday
    void setBirthday(std::string birthday);
    /// @brief get the identity's birthday
    /// @return the birthday ?
    std::string getBirthday();

    /// @brief set the identity's status from an integer value
    /// @param status the new status
    void setStatus(int s);
    /// @brief set the identity's status from a string value
    /// @param status the new status
    void setStatus(std::string s);
    /// @brief get the identity's status
    /// @return the status ?
    Identity::Status getStatus();

    void addContact(Contact::Type t, std::string detail);
    void addContact(std::string t, std::string detail);
    std::stack<Contact*>* getContacts();
    void removeContact(std::string contact, int index);
    void removeContacts();
    void printContacts();

    /// @brief add multiple notes
    /// @param n string containing every note separated by '_' character
    void addNotes(std::string n);
    /// @brief add a single note
    /// @param n string containing the note to add
    void addNote(std::string n);
    /// @brief get every notes as a single string
    /// @return THE string
    std::string getNotes();
    /// @brief remove every notes stored
    void removeNotes();
    /// @brief remove a single note if existing
    /// @param i index of the note to delete (1 being the highest note in the list)
    void removeNote(int i = 1);

    /// @brief print a resume of the identity informations
    void resume();
    friend std::ostream& operator<<(std::ostream& stream, const Identity& i);

private:
    /// @brief converter map from an identity status to its associated string
    static const std::map<Identity::Status, std::string> STATUS_TO_STRING;

    /// @brief a unique ID
    int id;
    /// @brief another name?
    std::string username;

    /// @brief what's your name?
    std::string name;
    /// @brief this is my proud lastname!
    std::string lastname;

    /// @brief uuuuh i hate getting old
    Date birthday;
    /// @brief age should be positiv right?
    int age;

    /// @brief am i alive??
    Identity::Status status;

    /// @brief a vector containing every known notes
    std::vector<std::string> notes;

    /// @brief a stack containing every known contacts
    std::stack<Contact*> contacts;
};
