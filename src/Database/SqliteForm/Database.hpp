//
//  Database.hpp
//  BankSystem2.0

#ifndef Database_hpp
#define Database_hpp

#include <string>
#include <iostream>
#include <sqlite3.h>

class Database {
private:
    sqlite3* db;
    std::string dbPath;
    
public:
    Database(const std::string& path);
    ~Database();
    
    sqlite3* getRawDB() const {
        return db;
    }
    
    bool open();
    void close();
    
    bool execute(const std::string& query);
    bool tableExists(const std::string& tableName);
};

#endif /* Database_hpp */
