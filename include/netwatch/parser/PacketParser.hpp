#pragma once
#include "../network/IPacketSniffer.hpp"
#include <optional>
#include <string>

namespace netwatch {

struct ParsedPacket {
    uint64_t timestamp;
    std::string src_ip;
    std::string dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    std::string protocol;
    size_t size;
};

class PacketParser {
public:
    std::optional<ParsedPacket> parse(const Packet& pkt);
};

} // namespace netwatch
