//
//  User.hpp
//  BankSystem2.0

#ifndef User_hpp
#define User_hpp

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <unordered_set>

#include "../Account/BankAccount.hpp"

using namespace std;

class User {
private:
    string password,passwordHash;
    string accountNumber;
    string userName, emailAddress, phoneNumber;
    double balance = 0.0;
    
    // Static set to keep track of generated account numbers
    static unordered_set<string> usedAccountNumbers;
    
    std::vector<BankAccount> accounts;
    
    // Helper function to generate a random 16-digit account number
    string generateAccountNumber();
    
    
public:
    User() : password(""), accountNumber(""), userName(""), emailAddress(""), phoneNumber("") { accountNumber = generateAccountNumber(); }; // Generate account number
    
    User(const string& userPassword, const string& userAccountNumber, const string& userName) : password(userPassword), accountNumber(userAccountNumber), userName(userName) {}
    
    void addAccount(const BankAccount& acc);
    
    // Get and Set password
    string getPassword() const;
    bool setPassword(const string &userPassword);
    
    // Get and set userName
    string getUserName() const;
    void setUserName(const string &name) {
        if (!name.empty()) {
            userName = name;
        }
    }
    
    // Get and set Account Number
    string getAccountNumber() const;
    void setAccountNumber();
    
    void setAccountNumber(const string& acc) {
        accountNumber = acc;
    }
    
    // get Accounts
    std::vector<BankAccount>& getAccounts();
    
    // Get and set Account Number
    string getEmailAddress() const;
    void setEmailAddress(const string& email) {
        if (!email.empty()) {
            emailAddress = email;
        }
    }
    
    string getPhoneNumber() const;
    void setPhoneNumber(const string& phone) {
        if (!phone.empty()) {
            phoneNumber = phone;
        }
    }
    
    double getBalance() const { return balance; };
    void setBalance(double b) { balance = b; }
    void deposit(double amount) { balance += amount; };
    bool withdraw(double amount) {
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }

    
protected:
    bool isValidPassword(const string& password) const
    {
        const size_t minLength = 8;
        const size_t maxLength = 128;
        
        if (password.length() < minLength || password.length() > maxLength) {
            cerr << "Length of password is short or too long\n";
            return false;
        }
        
        // Check for aleast one digit, one uppercase, and one lowercase
        bool hasDigit = false, hasUpper = false, hasLower = false;
        
        for (char c : password) {
            if (isdigit(c)) hasDigit = true;
            else if (isupper(c)) hasUpper = true;
            else if (islower(c)) hasLower = true;
        }
        
        return hasDigit && hasLower && hasUpper;
    }
    
    string hashPassword(const string& password)
    {
        // Use a proper crypto has function like bcrpyt or SHA-256
        // use library (pseudocode);
        // return bcrpyt_hash(password)
        
        return "hashed_" + password;
    }
};

#endif /* User_hpp */
