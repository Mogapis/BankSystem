
# Makefile
# BankSystem


# ============================
# Compiler + Flags
# ============================

CC = g++
CFLAGS = -Wall -std=c++17 -Iinclude -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lsqlite3

default: banksystem

banksystem: main.o include/User/User.o include/Repository/Users/UserRepository.o include/Repository/Accounts/AccountRepository.o Database/SqliteForm/Database.o include/Account/BankAccount.o include/Transaction/Transaction.o
	$(CC) -o banksystem main.o \
	include/User/User.o \
	include/Repository/Users/UserRepository.o \
	include/Repository/Accounts/AccountRepository.o \
	Database/SqliteForm/Database.o \
	include/Account/BankAccount.o \
	include/Transaction/Transaction.o \
	$(LDFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

include/User/User.o: include/User/User.cpp include/User/User.hpp
	$(CC) $(CFLAGS) -c include/User/User.cpp -o include/User/User.o

include/Repository/Users/UserRepository.o: include/Repository/Users/UserRepository.cpp include/Repository/Users/UserRepository.hpp
	$(CC) $(CFLAGS) -c include/Repository/Users/UserRepository.cpp -o include/Repository/Users/UserRepository.o
	
include/Repository/Accounts/AccountRepository.o: include/Repository/Accounts/AccountRepository.cpp
	include/Repository/Accounts/AccountRepository.hpp
	$(CC) $(CFLAGS) -c include/Repository/Accounts/AccountRepository.cpp -o include/Repository/Accounts/AccountRepository.o

Database/SqliteForm/Database.o: Database/SqliteForm/Database.cpp Database/SqliteForm/Database.hpp
	$(CC) $(CFLAGS) -c Database/SqliteForm/Database.cpp -o Database/SqliteForm/Database.o

include/Account/BankAccount.o: include/Account/BankAccount.cpp include/Account/BankAccount.hpp
	$(CC) $(CFLAGS) -c include/Account/BankAccount.cpp -o include/Account/BankAccount.o

include/Transaction/Transaction.o: include/Transaction/Transaction.cpp include/Transaction/Transaction.hpp
	$(CC) $(CFLAGS) -c include/Transaction/Transaction.cpp -o include/Transaction/Transaction.o

clean:
	find . -name "*.o" -type f -delete
	rm -f banksystem
