//
//  main.cpp
//  BankSystem

#include <iostream>
#include <limits>
#include <alloca.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <optional>

//Header Files
#include "include/Account/BankAccount.hpp"
#include "include/Transaction/Transaction.hpp"
#include "include/User/User.hpp"
#include "Database/SqliteForm/Database.hpp"
#include "include/Repository/Users/UserRepository.hpp"
#include "include/Repository/Accounts/AccountRepository.hpp"

using namespace std;

//static string username;
vector<User> users;

void menu(UserRepository& repo, AccountRepository& accRepo);
std::optional<User> Login(UserRepository& repo, AccountRepository& accRepo);
bool CreateAccount(UserRepository& repo, AccountRepository& accRepo);

void Dashboard(User& currentUser, UserRepository& repo, AccountRepository& accRepo);

int main(int argc, const char * argv[]) {
    
    Database db("/BankSystem/src/Database/SqliteForm/{databasename}.db");
    UserRepository repo(db, "/BankSystem/src/Database/Textform/userdata.txt");
    AccountRepository accRepo(db, "/BankSystem/src/Database/Textform/userdata.txt");
    
    repo.loadUsers();
    
    menu(repo, accRepo);
    
    return 0;
}


void menu(UserRepository& repo, AccountRepository& accRepo)
{
    
    int option1;
    
    cout << "------ Welcome to myBank System -------\n";
    
    do {
            cout << "|------------------------------------|\n";
            cout << "|          1. Login                  |\n";
            cout << "|          2. Create Account         |\n";
            cout << "|          3. Exit myBank            |\n";
            cout << "|____________________________________|\n" << endl;

            cout << "Enter option: ";

            if (!(cin >> option1)) { // Input validation for non-numeric input
                cout << "Invalid input. Please enter a number.\n";
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                continue;
            }

        switch (option1) {
            case 1: // Login
            {
                auto loggedInUser = Login(repo, accRepo);
                if (loggedInUser.has_value()) {
                    Dashboard(*loggedInUser, repo, accRepo);
                }
                break;
            }
            case 2: // Create
            {
                if (CreateAccount(repo, accRepo)) {
                    cout << "\nAccount created. Please login.\n";
                }
                
                break;
            }
                
            case 3:
                cout << "Exiting myBank. Goodbye!\n";
                break;
                
            default:
                if (option1 < 1 || option1 > 3) {
                   cout << "Invalid option. Please enter a number between 1 and 3.\n";
                   continue;
               }
                break;
        }

        } while (option1 != 3);
    
}

std::optional<User> Login(UserRepository& repo, AccountRepository& accRepo)
{
    string username, password;
    
    cout << "__Login__\n\n";
    
    cout << "Enter your username: ";
    cin >> username;
    
    cout << "Enter your password: ";
    cin >> password;
    
    auto userOpt = repo.findUserByUsername(username);
    
    if (userOpt.has_value()) {
        User user = userOpt.value();
        if (user.getPassword() == password) {
            cout << "\nLogin successful! Welcome " << user.getUserName() << "!\n";
            cout << "Account Number: " << user.getAccountNumber() << "\n\n";
            return user; 
        }
    }
    
    cout << "\nLogin failed. Incorrect username or password.\n";
    return std::nullopt;
}

bool CreateAccount(UserRepository& repo, AccountRepository& accRepo)
{
    User newUser;
    string username, password, email, phoneNumber;
    
    cout << "\n--- Create Account ---\n";
    
    cout << "Enter a username: ";
    cin >> username;
    
    if (repo.findUserByUsername(username)) {
        cout << "Username already exists. Try another.\n";
        return false;
    }
    
    cout << "Enter a password: ";
    cin >> password;
    
    if (!newUser.setPassword(password)) {
        cout << "Password must be at least 8 characters and include an uppercase letter, lowercase letter, and a digit.\n";
        return false;
    }
    
    cout << "Enter your email address: ";
    cin >> email;
    
    
    cout << "Enter your phone number: ";
    cin >> phoneNumber;

    // Use setters to store user details
    newUser.setUserName(username);
    newUser.setEmailAddress(email);
    newUser.setPhoneNumber(phoneNumber);

    if (repo.addUser(newUser)) {
        cout << "\nAccount created successfully!\n";
        cout << "Username: " << newUser.getUserName() << "\n";
        cout << "Account Number: " << newUser.getAccountNumber() << "\n";
        return true;
    }
    
    return false;
}

void Dashboard(User& currentUser, UserRepository& repo, AccountRepository& accRepo)
{
    Transaction txn;
    int option;
    
    do {
        
            cout << "\n==============================\n";
            cout << "   Welcome, " << currentUser.getUserName() << "\n";
            cout << "==============================\n";
            
            cout << "1. View Account Info\n";
            cout << "2. Check Balance\n";
            cout << "3. Deposit\n";
            cout << "4. Withdraw\n";
            cout << "5. Transfer\n";
            cout << "6. Transaction History\n";
            cout << "7. Add a Savings account\n";
            cout << "8. Logout\n";
            
            cout << "Choose option: ";
            cin >> option;
        
        switch (option) {
            case 1:
                cout << "\n--- Account Info ---\n";
                cout << "Username: " << currentUser.getUserName() << endl;
                cout << "Account Number: " << currentUser.getAccountNumber() << endl;
                cout << "Email: " << currentUser.getEmailAddress() << endl;
                cout << "Phone: " << currentUser.getPhoneNumber() << endl;
                
                break;
                
            case 2:
                cout << "\n--- Your Balance ---\n";
                txn.showBalance(currentUser);
                break;
                
            case 3:
                cout << "\n--- Deposit---\n";
                txn.deposit(currentUser, repo );
                break;
                
            case 4:
                cout << "\n--- Withdraw ---\n";
                txn.withdraw(currentUser);
                break;
                
            case 7:
                accRepo.loadAccounts();
                
                accRepo.createAccount(currentUser.getUserName(), "SAVINGS");
                break;
                
            case 8:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid option.\n";
        }
        
        
    } while (option != 8);
}

/*
 
 void LoadUsersFromFile()
 {
     ifstream file(fileData);
     string line;

     if (!file.is_open()) {
         cout << "No existing user database found.\n";
         return;
     }

     while (getline(file, line))
     {
         // Skip separator/header lines
         if (line.find("Account Number") != string::npos) continue;
         if (line.find("----") != string::npos) continue;
         if (line.size() < 5) continue;

         // Only process real user rows starting with "|"
         if (line[0] != '|') continue;

         // Remove pipes
         stringstream ss(line);
         string temp;

         User loadedUser;

         string no, accNum, uname, pass, email, phone;

         getline(ss, temp, '|'); // empty
         getline(ss, no, '|');
         getline(ss, accNum, '|');
         getline(ss, uname, '|');
         getline(ss, pass, '|');
         getline(ss, email, '|');
         getline(ss, phone, '|');

         // Trim spaces
         auto trim = [](string &s) {
             s.erase(0, s.find_first_not_of(" "));
             s.erase(s.find_last_not_of(" ") + 1);
         };

         trim(accNum);
         trim(uname);
         trim(pass);
         trim(email);
         trim(phone);

         // Fill user object
         loadedUser.setUserName(uname);
         loadedUser.setEmailAddress(email);
         loadedUser.setPhoneNumber(phone);

         // Store password directly (temporary)
         loadedUser.setPassword(pass);

         users.push_back(loadedUser);
     }

     file.close();

     cout << "Users Loaded users from file.\n";
 }

 
*/
