#include <iostream>
#include <fstream>
#include <memory>
#include <string.h>
#include "sqlite3.h"

#define INPUTDATA "trips.txt"
#define DATABASE "ttcdata.sqlite"
#define TABLE "CREATE TABLE IF NOT EXISTS TTC (id INTEGER PRIMARY KEY, Route_ID TEXT, Service_Id INTEGER, Trip_Id INTEGER, Trip_Headsign TEXT, Trip_Short_Name TEXT, Direction_Id Integer, Block_Id INTEGER)"

int main(int argc, char **argv) {

    sqlite3 * db;
    sqlite3_stmt * stmt;
    char * sErrMsg = 0;
    const char * tail = 0;
    int n = 0;
    char sSQL [256] = "\0";

    sqlite3_open("ttcdata.sqlite", &db);
    sqlite3_exec(db, TABLE, NULL, NULL, &sErrMsg);
    if (argc > 1 && (atoi(argv[1]) & 2))
        sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &sErrMsg);
    if (argc > 1 && (atoi(argv[1]) & 4))
        sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &sErrMsg);
    if (argc > 1 && (atoi(argv[1]) & 8))
        sqlite3_exec(db, "PRAGMA page_size = 16000", NULL, NULL, &sErrMsg);

    sprintf(sSQL, "INSERT INTO TTC VALUES (NULL, @RT, @BR, @VR, @ST, @VI, @DT, @TM)");
    sqlite3_prepare_v2(db,  sSQL, 256, &stmt, &tail);

    auto inputf = std::make_unique<std::ifstream>("trips.txtaa",std::ios::binary);
    std::string line;
    char *temp = nullptr;
    while (std::getline(*inputf,line)) {
        if (argc > 1 && (atoi(argv[1]) & 1))
            if (n%100 == 1) {
                sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
            }
        sqlite3_bind_text(stmt, 1, (temp = strtok(const_cast<char*>(line.data()),",")), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, (temp = strtok (temp, ",")), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, (temp = strtok (temp, ",")), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, (temp = strtok (temp, ",")), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, (temp = strtok (temp, ",")), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, (temp = strtok (temp, ",")), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, (temp = strtok (temp, ",")), -1, SQLITE_TRANSIENT);
        
        sqlite3_step(stmt);
        if (argc > 1 && (atoi(argv[1]) & 1))
            if (n%100 == 0) {
                sqlite3_exec(db, "COMMIT", NULL, NULL, &sErrMsg);
            }
        n++;
    }
    sqlite3_exec(db, "COMMIT", NULL, NULL, &sErrMsg);
    sqlite3_close(db);
    return 0;
}