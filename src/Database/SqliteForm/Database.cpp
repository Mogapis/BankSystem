//
//  Database.cpp
//  BankSystem2.0
//
//  Created by Khutso Sekele on 2026/02/07.
//

#include "Database.hpp"

using namespace std;

Database::Database(const string& path) : db(nullptr), dbPath(path) {}

Database::~Database() {
    close();
}

bool Database::open()
{
    if (sqlite3_open(dbPath.c_str(), &db) == SQLITE_OK) {
        return true;
    } else {
        cerr << "Failed to open SQLite DB: " << sqlite3_errmsg(db) << endl;
        db = nullptr;
        return false;
    }
}

void Database::close()
{
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::execute(const std::string &query)
{
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "SQLite Error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}
 
bool Database::tableExists(const std::string &tableName)
{
    sqlite3_stmt* stmt;
    string sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + tableName + "';";
    if (sqlite3_prepare(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;
    bool exits = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exits;
}
