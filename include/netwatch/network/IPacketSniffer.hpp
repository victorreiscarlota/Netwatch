#pragma once
#include <vector>
#include <cstdint>
#include <functional>
#include <string>

namespace netwatch {

struct Packet {
    uint64_t timestamp;
    std::vector<uint8_t> data;
};

class IPacketSniffer {
public:
    virtual ~IPacketSniffer() = default;
    virtual void startCapture(const std::string& interface) = 0;
    virtual void stopCapture() = 0;
    virtual void setPacketHandler(std::function<void(const Packet&)> handler) = 0;
};

} // namespace netwatch
