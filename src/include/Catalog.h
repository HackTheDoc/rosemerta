#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

class Identity;

class Catalog {
private:
    // items stored by id
    std::unordered_map<int, Identity*> items;

    void loadIdentities();
    void loadContacts();
    void loadLocations();

public:
    Catalog();
    ~Catalog();

    void load();

    Identity* at(const int& id);

    void clear();

    std::unordered_map<int, Identity*>::iterator begin() noexcept;
    std::unordered_map<int, Identity*>::iterator end() noexcept;
    std::unordered_map<int, Identity*>::const_iterator begin() const noexcept;
    std::unordered_map<int, Identity*>::const_iterator end() const noexcept;

    friend std::ostream& operator<<(std::ostream& stream, const Catalog& c);
};
