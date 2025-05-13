#include "netwatch/storage/SQLiteStorage.hpp"
#include <iostream>
#include <sstream>

namespace netwatch {

SQLiteStorage::SQLiteStorage(const std::string& dbPath) : db_(nullptr) {
    if (sqlite3_open(dbPath.c_str(), &db_) != SQLITE_OK) {
        std::cerr << "Error opening SQLite DB: " << sqlite3_errmsg(db_) << "\n";
    }
    initDB();
}

SQLiteStorage::~SQLiteStorage() {
    if (db_) sqlite3_close(db_);
}

void SQLiteStorage::initDB() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS packets (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp INTEGER,
            src_ip TEXT,
            dst_ip TEXT,
            src_port INTEGER,
            dst_port INTEGER,
            protocol TEXT,
            size INTEGER
        );
    )";
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQLite error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

void SQLiteStorage::persistPacket(const Packet&) {
    // Ignorado para ParsedPacket version
}

void SQLiteStorage::persistPacket(const ParsedPacket& pkt) {
    std::stringstream ss;
    ss << "INSERT INTO packets (timestamp, src_ip, dst_ip, src_port, dst_port, protocol, size) VALUES ("
       << pkt.timestamp << ", "
       << "'" << pkt.src_ip << "', "
       << "'" << pkt.dst_ip << "', "
       << pkt.src_port << ", "
       << pkt.dst_port << ", "
       << "'" << pkt.protocol << "', "
       << pkt.size << ");";

    std::string query = ss.str();
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQLite insert error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

} // namespace netwatch
