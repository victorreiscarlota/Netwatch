#pragma once
#include "netwatch/network/IPacketSniffer.hpp"
#include <thread>
#include <functional>
#include <string>

namespace netwatch {

class BoostSniffer : public IPacketSniffer {
public:
    BoostSniffer();
    ~BoostSniffer();

    void startCapture(const std::string& interface) override;
    void stopCapture() override;
    void setPacketHandler(std::function<void(const Packet&)> handler) override;

private:
    void captureLoop();
    std::string interface_;
    std::function<void(const Packet&)> handler_;
    std::thread thread_;
    bool running_ = false;
};

} // namespace netwatch
