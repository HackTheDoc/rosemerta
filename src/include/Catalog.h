#pragma once

#include <iostream>
#include <string>
#include <set>
#include <unordered_map>

class Identity;
class Contact;

/// @brief A class representing a map of id and identities pair of objects
class Catalog {
public:
    /// @brief the different existing types of changes stored in the changelog
    enum ChangelogType {
        NEW_IDENTITY,
        USERNAME,
        NAME,
        LASTNAME,
        AGE,
        BIRTHDAY,
        STATUS,
        NOTES,
        ERASE,
        UPDATE_CONTACT
    };

    static std::set<std::pair<int, ChangelogType>> changelog;

    Catalog();
    ~Catalog();

    /// @brief load the catalog from the application's referenced database
    void load();

    /// @brief save modifications placed in the changelog
    void save();

    /// @brief insert a new element in the catalog
    /// @param i pointer to the element (identity) to add
    void insert(Identity* i);

    /// @brief get an identity stored in the catalog
    /// @param id id of the requied identity
    /// @return pointer to the required identity if founded, else nullptr
    Identity* at(const int& id);

    /// @brief find the IDs related to a name, a lastname or a username
    /// @param name string containing the name, the lastname or the username of the person we're searching for
    /// @return a set of id (empty set if there is no id founded)
    std::set<int> findID(std::string name);

    /// @brief count the number of occurence of an id
    /// @param id id to count
    /// @return the number of occurence of the required id, else 0
    int count(const int& id);

    /// @brief erase an identity from the catalog
    /// @param id id of the identity
    void erase(const int& id);
    /// @brief remove one specific contact of an identity
    /// @param id id of the identity
    /// @param type type of contact to remove
    /// @param index index of the contact to remove (default: 1)
    void removeContact(const int& id, std::string type, int index = 1);
    /// @brief remove every contact of an identity
    /// @param id id of the identity
    void removeContacts(const int& id);

    /// @brief clear the catalog from all of its items
    void clear();

    /// @brief display a minimal version of the catalog
    void display();

private:
    /// @brief items stored by id
    std::unordered_map<int, Identity*> items;
    /// @brief deleted items stored by their old id
    std::unordered_map<int, Identity*> deletedItems;
    /// @brief capacity originately loaded when executing first load of the catalog
    int loadedCapacity;

    /// @brief get the next unused id
    /// @return an integer representing the next id
    int nextFreeID();

    /// @brief load identities from the appropriate table of the application's database
    /// @return true if successfully loaded, else false
    bool loadIdentities();
    /// @brief load contacts from the appropriate table of the application's database
    /// @return true if successfully loaded, else false
    bool loadContacts();
    /// @brief load locations from the appropriate table of the application's database
    /// @return true if successfully loaded, else false
    bool loadLocations();

    /// @brief update one username in the database
    /// @param id id of the identity to update 
    void updateUsername(int id);
    /// @brief update one name in the database
    /// @param id id of the identity to update 
    void updateName(int id);
    /// @brief update one lastname in the database
    /// @param id id of the identity to update 
    void updateLastname(int id);
    /// @brief update one age in the database
    /// @param id id of the identity to update 
    void updateAge(int id);
    /// @brief update one birthday in the database
    /// @param id id of the identity to update 
    void updateBirthday(int id);
    /// @brief update one status in the database
    /// @param id id of the identity to update 
    void updateStatus(int id);
    /// @brief update one nots in the database
    /// @param id id of the identity to update 
    void updateNotes(int id);
    /// @brief delete one from database
    /// @param id id of the identity to erase 
    void eraseID(int id);
    /// @brief update one contacts in the database
    /// @param id id of the identity to update 
    void updateContacts(int id);
    /// @brief save new one to in the database
    /// @param id id of the identity to update 
    void saveIdentity(int id);

    /// @brief find existing IDs from a username
    /// @param username 
    /// @return a set containing the founded IDs
    std::set<int> findByUsername(std::string username);
    /// @brief find existing IDs from a name
    /// @param name 
    /// @return a set containing the founded IDs
    std::set<int> findByName(std::string name);
    /// @brief find existing IDs from a lastname
    /// @param lastname 
    /// @return a set containing the founded IDs
    std::set<int> findByLastname(std::string lastname);
    /// @brief find existing IDs from a firstname
    /// @param firstname 
    /// @return a set containing the founded IDs
    std::set<int> findByFirstname(std::string firstname);
};
