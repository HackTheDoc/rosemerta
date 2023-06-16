#pragma once

#include <iostream>
#include <string>
#include <set>
#include <unordered_map>

class Identity;
class Contact;

class Catalog {
public:
    enum ChangingType {
        NEW_IDENTITY,
        USERNAME,
        NAME,
        LASTNAME,
        AGE,
        BIRTHDAY,
        STATUS
    };

    static std::set<std::pair<int, ChangingType>> changelog;

    Catalog();
    ~Catalog();

    void load();

    void save();

    Identity* at(const int& id);

    std::set<int> findID(std::string name);

    int count(const int& id);

    void clear();

    std::unordered_map<int, Identity*>::iterator begin() noexcept;
    std::unordered_map<int, Identity*>::iterator end() noexcept;
    std::unordered_map<int, Identity*>::const_iterator begin() const noexcept;
    std::unordered_map<int, Identity*>::const_iterator end() const noexcept;

    friend std::ostream& operator<<(std::ostream& stream, const Catalog& c);

private:
    // items stored by id
    std::unordered_map<int, Identity*> items;
    int loadedCapacity;

    void loadIdentities();
    void loadContacts();
    void loadLocations();

    void updateUsername(int id);
    void updateName(int id);
    void updateLastname(int id);
    void updateAge(int id);
    void updateBirthday(int id);
    void updateStatus(int id);

    std::set<int> findByUsername(std::string username);
    std::set<int> findByName(std::string name);
    std::set<int> findByLastname(std::string lastname);
    std::set<int> findByFirstname(std::string firstname);
};
