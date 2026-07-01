//
//  AccountRepository.cpp
//  BankSystem2.0

#include "AccountRepository.hpp"
#include <iostream>

using namespace std;

AccountRepository::AccountRepository(Database& database, const std::string& backupPath): db(database) {}

void AccountRepository::loadAccounts()
{
    if (!db.open()) {
        return;
    }
    
    // Create Account table
    db.execute(
            "CREATE TABLE IF NOT EXISTS accounts ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "accountNumber TEXT UNIQUE,"
            "userId TEXT,"
            "accountType TEXT,"
            "balance REAL DEFAULT 0,"
            "FOREIGN KEY(userId) REFERENCES users(username)"
            ");"
        );
    
    // Load accounts
    sqlite3_stmt* stmt;
    string sql = "SELECT accountNumber, userId, accountType, balance FROM accounts;";
    
    if (sqlite3_prepare_v2(db.getRawDB(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to read accounts from database.\n";
        return;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        BankAccount acc;
        
        acc.setAccountNumber((const char*)sqlite3_column_text(stmt, 0));
        acc.setUserId((const char*)sqlite3_column_text(stmt, 1));
        acc.setAccountType((const char*)sqlite3_column_text(stmt, 2));
        acc.setBalance(sqlite3_column_double(stmt, 3));
        
        accounts.push_back(acc);
    }
    
    sqlite3_finalize(stmt);
    
    cout << "Loaded accounts from SQLite successfully.\n";
}

bool AccountRepository::createAccount(const std::string &userId, const std::string &accountType)
{
    
    // Validate account type
    if (accountType != "SAVINGS" && accountType != "CHEQUE") {
        cout << "Invalid account type.\n";
        return false;
    }
    
    // Count existing account for this user
    int savingsCount = 0;
    int chequeCount = 0;
    
    for (const auto& acc : accounts)
    {
        if (acc.getUserId() == userId) {
            if (acc.getAccountType() == "SAVINGS") savingsCount++;
            if (acc.getAccountType() == "CHEQUE") chequeCount++;
        }
    }
    
    // limits
    if (accountType == "SAVINGS" && savingsCount >= 5) {
        cout << "Max 5 savings accounts allowed.\n";
        return false;
    }
    if (accountType == "CHEQUE" && chequeCount >= 2) {
        cout << "Max 2 cheque accounts allowed.\n";
        return false;
    }
    
    BankAccount acc;
    
    acc.setAccountNumber(acc.generateAccountNumber());
    acc.setUserId(userId);
    acc.setAccountType(accountType);
    acc.setBalance(0.0);
    
    accounts.push_back(acc);
    
    if (!db.open()) {
        cout << "Database unavailable.\n";
        return false;;
    }
    
    string sql = "INSERT INTO accounts (accountNumber, userId, accountType, balance) VALUES (?, ?, ?, ?);";
    
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db.getRawDB(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cout << "Preparation failed.\n";
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, acc.getAccountNumber().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, userId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, accountType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, 0.0);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << "Failed to insert account.\n";
        sqlite3_finalize(stmt);
        return false;
    }
    
    sqlite3_finalize(stmt);
    
    cout << "Account created! Account Number: " << acc.getAccountNumber() << " and type " << acc.getAccountType() << "\n";
    
    return true;
}
