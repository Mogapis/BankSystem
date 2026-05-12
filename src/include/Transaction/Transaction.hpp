//
//  Transaction.hpp
//  BankSystem2.0
//
//  Created by Khutso on 2025/05/02.
//

#ifndef Transaction_hpp
#define Transaction_hpp

#include <stdio.h>
#include <string>

#include "../User/User.hpp"
#include "../Repository/Users/UserRepository.hpp"

class Transaction {

public:
    Transaction();
    void showBalance(User& user);
    void deposit(User& user, UserRepository& repo);
    void withdraw(User& user);
    
    double savingsAccount(double amount, double balance);
    void beneficiary();
};

#endif /* Transaction_hpp */
