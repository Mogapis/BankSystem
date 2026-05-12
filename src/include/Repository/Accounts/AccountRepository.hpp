//
//  AccountRepository.hpp
//  BankSystem2.0
//
//  Created by Khutso Sekele on 2026/04/05.
//

#ifndef AccountRepository_hpp
#define AccountRepository_hpp

#include <stdio.h>

#include "../../User/User.hpp"
#include "../../Account/BankAccount.hpp"
#include "../../../Database/SqliteForm/Database.hpp"

#include <vector>
#include <string>
#include <optional>

class AccountRepository {
private:
    Database& db;
    std::vector<BankAccount> accounts;
    
public:
    AccountRepository(Database& database, const std::string& backupPath);
    // TODO: For the backupPath it will store how many accounts the user has.
    
    std::vector<BankAccount> getAccountByUser(const std::string& userId);
    
    void loadAccounts();
    bool createAccount(const std::string& userId, const std::string& accountType);
    bool updateBalance(const std::string& accountNumber, double newBalance);
    
};


#endif /* AccountRepository_hpp */
