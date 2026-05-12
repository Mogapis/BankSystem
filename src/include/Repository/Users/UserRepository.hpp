//
//  UserRepository.hpp
//  BankSystem2.0
//
//  Created by Khutso Sekele on 2026/02/07.
//

#pragma once

#ifndef UserRepository_hpp
#define UserRepository_hpp

#include "../../User/User.hpp"
#include "../../../Database/SqliteForm/Database.hpp"

#include <vector>
#include <string>
#include <optional>

class UserRepository {
private:
    Database& db;
    std::string backupFile;
    std::vector<User> users;
    
    void loadFromBackup();
    void saveToBackup(const User& user);
    
public:
    UserRepository(Database& database, const std::string& backupPath);
    
    void loadUsers();
    bool addUser(const User& user);
    std::optional<User> findUserByUsername(const std::string& username);
    const std::vector<User>& getAllUsers() const;
    bool updateBalance(const std::string& username, double newBalance);
    
};

#endif /* UserRepository_hpp */
