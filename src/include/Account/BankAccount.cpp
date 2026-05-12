//
//  BankAccount.cpp
//  BankSystem2.0
//
//  Created by Khutso on 2025/05/02.
//

#include "BankAccount.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

BankAccount::BankAccount() {};

BankAccount::BankAccount(int accNo, string name, string type, double initialBalance, string pwd) : accountHolderName(name), accountType(type), password(pwd)
{
    accountNumber = generateAccountNumber();
}


string BankAccount::generateAccountNumber()
{
    static std::unordered_set<std::string> usedAccountNumbers;
    
    random_device rd;                       //Obtain a random number from hardware
    mt19937 gen(rd());                      // Seed the generator
    uniform_int_distribution<> dist(0, 9);  // Define the range (0. 9)
    
    string accountNum = "";
    
    do {
        accountNum = ""; // Reset account number
        
        for (int i = 0; i < 16; i++) {
            accountNum += to_string(dist(gen));
        }
    } while (usedAccountNumbers.find(accountNum) != usedAccountNumbers.end()); // ensure each number generated is unique
    
    usedAccountNumbers.insert(accountNum); // Mark as used
    
    return accountNum;
}

void BankAccount::setAccountNumber(const string& acc)
{
    accountNumber = acc;
}

string BankAccount::getAccountNumber() const
{
    return accountNumber;
}

string BankAccount::getAccountHolderName() const
{
    return accountHolderName;
}


void BankAccount::setAccountHolderName(const std::string& name)
{
    if (!name.empty()) {
        accountHolderName = name;
    }
}

void BankAccount::setUserId(const std::string& id)
{
    if (!id.empty()) {
        userId = id;
    }
}

void BankAccount::setAccountType(const std::string& type)
{
    // Accept only SAVINGS or CHEQUE
    if (type == "SAVINGS" || type == "CHEQUE") {
        accountType = type;
    }
}

void BankAccount::setBalance(double b)
{
    balance = b;
}

std::string BankAccount::getUserId() const
{
    return userId;
}

std::string BankAccount::getAccountType() const
{
    return accountType;
}

double BankAccount::getBalance() const
{
    return balance;
}

void BankAccount::showBeneficiaries() const
{
    cout << "Beneficiaries (" << beneficiaries.size() << "):\n";
    for (const auto& b : beneficiaries) {
        cout << " - " << b << '\n';
    }
}

void BankAccount::removeBeneficiary(const std::string& beneficiary)
{
    auto it = std::remove(beneficiaries.begin(), beneficiaries.end(), beneficiary);
    if (it != beneficiaries.end()) {
        beneficiaries.erase(it, beneficiaries.end());
        cout << beneficiary << " removed from beneficiaries.\n";
    } else {
        cout << beneficiary << " not found among beneficiaries.\n";
    }
}

bool BankAccount::authenticate(const std::string& inputPassword) const
{
    return inputPassword == password;
}

void BankAccount::addBeneficiaries(const std::string &name)
{
    beneficiaries.push_back(name);
    cout << name << " added as a beneficiary.\n"; 
}
