//
//  UserRepository.cpp
//  BankSystem2.0

#include "UserRepository.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

UserRepository::UserRepository(Database& database, const std::string& backupPath)
    : db(database), backupFile(backupPath) {}

void UserRepository::loadFromBackup()
{
    ifstream file(backupFile);
    if (!file.is_open()) return;
    
    string line;
    while (getline(file, line)) {
        if (line.find("Account Number") != string::npos || line.find("---") != string::npos) continue;
        if (line.empty() || line[0] != '|') continue;
        
        stringstream ss(line);
        string temp, acc, uname, pass, email, phone;
        getline(ss, temp, '|');
        getline(ss, temp, '|');
        getline(ss, acc, '|');
        getline(ss, uname, '|');
        getline(ss, pass, '|');
        getline(ss, email, '|');
        getline(ss, phone, '|');
        
        auto trim = [](string &s){ s.erase(0,s.find_first_not_of(" ")); s.erase(s.find_last_not_of(" ")+1); };
        trim(acc); trim(uname); trim(pass); trim(email); trim(phone);
        
        User u;
        u.setAccountNumber(acc);
        u.setUserName(uname);
        u.setEmailAddress(email);
        u.setPhoneNumber(phone);
        u.setPassword(pass);
        
        users.push_back(u);
    }
}

void UserRepository::saveToBackup(const User& user)
{
    ofstream out(backupFile, ios::app);
    if (!out.is_open()) return;
    
    out << "| " << std::setw(5) << users.size()+1
        << "| " << std::setw(25) << user.getAccountNumber()
        << "| " << std::setw(25) << user.getUserName()
        << "| " << std::setw(25) << user.getPassword()
        << "| " << std::setw(25) << user.getEmailAddress()
        << "| " << std::setw(15) << user.getPhoneNumber()
        << "|\n";
    
    out.close();
}

void UserRepository::loadUsers()
{
    if (!db.open()) {
        cout << "Database unavailable. Loading from txt backup...\n";
        loadFromBackup();
        return;
    }
    
    // Create table if not exists
    db.execute(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "accountNumber TEXT UNIQUE,"
        "username TEXT UNIQUE,"
        "password TEXT,"
        "email TEXT,"
        "phone TEXT,"
        "balance REAL DEFAULT 0"
        ");"
    );
    
    // Load all users
    sqlite3_stmt* stmt;
    string sql = "SELECT accountNumber, username, password, email, phone, balance FROM users;";
    
    if (sqlite3_prepare_v2(db.getRawDB(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to read users from database.\n";
        return;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        User u;
        
        u.setAccountNumber((const char*)sqlite3_column_text(stmt, 0));
        u.setUserName((const char*)sqlite3_column_text(stmt, 1));
        u.setPassword((const char*)sqlite3_column_text(stmt, 2));
        u.setEmailAddress((const char*)sqlite3_column_text(stmt, 3));
        u.setPhoneNumber((const char*)sqlite3_column_text(stmt, 4));
        u.setBalance(sqlite3_column_double(stmt, 5));
        
        users.push_back(u);
    }
    
    sqlite3_finalize(stmt);
    
    cout << "Loaded users from SQLite successfully.\n";
}

bool UserRepository::addUser(const User &user)
{
    users.push_back(user);
    
    if (!db.open()) {
        cout << "Database unavailable. Saving to txt backup...\n";
        saveToBackup(user);
        return true;
    }
    
    string sql = "INSERT INTO users (accountNumber, username, password, email, phone, balance) VALUES (?, ?, ?, ?, ?, ?);";
    
    sqlite3_stmt* stmt;
    
    sqlite3_prepare_v2(db.getRawDB(), sql.c_str(), -1, &stmt, nullptr);
    
    sqlite3_bind_text(stmt, 1, user.getAccountNumber().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.getUserName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.getEmailAddress().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, user.getPhoneNumber().c_str(), -1, SQLITE_TRANSIENT);
    
    sqlite3_bind_double(stmt, 6, user.getBalance());
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << "Failed to insert user (username may already exist).\n";
        sqlite3_finalize(stmt);
        return false;
    }
    
    sqlite3_finalize(stmt);
    
    cout << "User saved in SQLite successfully!\n";
    
    saveToBackup(user);
    
    return true;
}

optional<User> UserRepository::findUserByUsername(const std::string &username)
{
    for (const auto& u : users) {
        if (u.getUserName() == username) return u;
    }
    return nullopt;
}

const std::vector<User>& UserRepository::getAllUsers() const { return users; }

bool UserRepository::updateBalance(const std::string &username, double newBalance)
{
    if (!db.open()) {
        cout << "Database unavailable. Cannot update balance.\n";
        return false;
    }
    
    string sql = "UPDATE users SET balance = ? WHERE username = ?;";
    
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db.getRawDB(), sql.c_str(), -1, &stmt, nullptr);
    
    sqlite3_bind_double(stmt, 1, newBalance);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << "Failed to update balance.\n";
        sqlite3_finalize(stmt);
        return false;
    }
    
    sqlite3_finalize(stmt);
    
    for (auto &u : users) {
        if (u.getUserName() == username) {
            u.setBalance(newBalance);
            break;
        }
    }
    return true;
}

