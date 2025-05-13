#pragma once
#include "../network/IPacketSniffer.hpp"
#include "../parser/PacketParser.hpp"
#include <sqlite3.h>
#include <string>

namespace netwatch {

class IPacketStorage {
public:
    virtual ~IPacketStorage() = default;
    virtual void persistPacket(const Packet& pkt) = 0;
};

class SQLiteStorage : public IPacketStorage {
public:
    SQLiteStorage(const std::string& dbPath);
    ~SQLiteStorage();

    void persistPacket(const Packet& pkt) override;
    void persistPacket(const ParsedPacket& pkt);

private:
    sqlite3* db_;
    void initDB();
};

} // namespace netwatch
