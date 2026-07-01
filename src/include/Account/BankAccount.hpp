//
//  BankAccount.hpp
//  BankSystem2.0

#ifndef BankAccount_hpp
#define BankAccount_hpp

#include <stdio.h>
#include <string>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <vector>

class BankAccount {
private:
    std::string accountNumber;
    std::string accountHolderName;
    std::string userId;
    std::string accountType; // "SAVINGS" or "CHEQUE"
    std::string password;

    //std::vector<Transaction>;
    std::vector<std::string> beneficiaries;
    
    double balance;
    

    
    
public:
    BankAccount();
    BankAccount(int accNo, std::string name, std::string accountType, double initialBalance, std::string pwd); // accountType is if savings or cheque (Credit or Debit)
    
    std::string generateAccountNumber();
    void setAccountNumber(const std::string& acc);
    std::string getAccountNumber() const;
    
    void setAccountHolderName(const std::string& name);
    void setUserId(const std::string& id);
    void setAccountType(const std::string& type);
    void setBalance(double b);
    
    std::string getAccountHolderName() const;
    std::string getUserId() const;
    std::string getAccountType() const;
    double getBalance() const;
    void addBeneficiaries(const std::string& name);
    void showBeneficiaries() const;
    void removeBeneficiary(const std::string& beneficiary);
    bool authenticate(const std::string& inputPassword) const;
};

#endif /* BankAccount_hpp */
