#pragma once
#include "netwatch/network/IPacketSniffer.hpp"

namespace netwatch {

class IDataStorage {
public:
    virtual ~IDataStorage() = default;
    virtual void persistPacket(const Packet& pkt) = 0;
};

} // namespace netwatch
