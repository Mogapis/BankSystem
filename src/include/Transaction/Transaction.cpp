//
//  Transaction.cpp
//  BankSystem2.0
//
//  Created by Khutso on 2025/05/02.
//

#include "Transaction.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

Transaction::Transaction()
{
   
}


void Transaction::showBalance(User& user)
{
    cout << "Your current balance is: R" << fixed << setprecision(2) << user.getBalance() << endl;
}

void Transaction::deposit(User& user, UserRepository& repo)
{
    double amount;
    
    cout << "Enter the amount you want deposit R";
    cin >> amount;
    
    if (amount <= 0) {
        cout << "Invalid deposit amount.\n";
        return;
    }

    user.deposit(amount);
    
    repo.updateBalance(user.getUserName(), user.getBalance());
    
    cout << "Deposit successfully!\n";
    showBalance(user);
}


void Transaction::withdraw(User &user)
{
    double amount;
    cout << "Enter withdrawal amount: R";
    cin >> amount;
    
    if (amount <= 0) {
        cout << "Invalid amount.\n";
        return;
    }
    
    if (!user.withdraw(amount)) {
        cout << "Insufficient funds!\n";
        return;
    }
    cout << "Withdrawl successful!\n";
    showBalance(user);
}

