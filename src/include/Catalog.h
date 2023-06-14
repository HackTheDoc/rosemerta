#pragma once

#include <iostream>
#include <string>
#include <set>
#include <unordered_map>

class Identity;

class Catalog {
private:
    // items stored by id
    std::unordered_map<int, Identity*> items;

    void loadIdentities();
    void loadContacts();
    void loadLocations();

    std::set<int> findByUsername(std::string username);
    std::set<int> findByName(std::string name);
    std::set<int> findByLastname(std::string lastname);
    std::set<int> findByFirstname(std::string firstname);

public:
    Catalog();
    ~Catalog();

    void load();

    Identity* at(const int& id);

    std::set<int> findID(std::string name);

    int count(const int& id);

    void clear();

    std::unordered_map<int, Identity*>::iterator begin() noexcept;
    std::unordered_map<int, Identity*>::iterator end() noexcept;
    std::unordered_map<int, Identity*>::const_iterator begin() const noexcept;
    std::unordered_map<int, Identity*>::const_iterator end() const noexcept;

    friend std::ostream& operator<<(std::ostream& stream, const Catalog& c);
};
