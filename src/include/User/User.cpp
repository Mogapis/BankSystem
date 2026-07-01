//
//  User.cpp
//  BankSystem2.0

#include "User.hpp"

// Define the static member
unordered_set<string> User::usedAccountNumbers;

string User::generateAccountNumber()
{
    random_device rd;                            // Obtain a random number from hardware
    mt19937 gen(rd());                          // Seed the generator
    uniform_int_distribution<> dist(0, 9);      // Define the range (0-9)
    
    string accountNum = "";
    
    do {
        accountNum = "";    // Reset account number
        
        for (int i = 0; i < 16; i++) {
            accountNum += to_string(dist(gen));
        }
        
    } while (usedAccountNumbers.find(accountNum) != usedAccountNumbers.end()); //Ensures there is no dupilications
    
    usedAccountNumbers.insert(accountNum); // Mard as used
    
    return accountNum;
}

string User::getPassword() const
{
    return password;
}

bool User::setPassword(const string &userPassword)
{
    if (!isValidPassword(userPassword)) {
        return false;
    }
    password = userPassword;
    passwordHash = hashPassword(userPassword);
    
    return true;
}

string User::getAccountNumber() const
{
    return accountNumber;
}

void User::setAccountNumber()
{
    accountNumber = generateAccountNumber();
}

string User::getUserName() const
{
    return userName;
}

string User::getEmailAddress() const
{
    return emailAddress;
}

string User::getPhoneNumber() const
{
    return phoneNumber;
}
