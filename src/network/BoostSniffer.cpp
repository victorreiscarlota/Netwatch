// src/network/BoostSniffer.cpp

#include "netwatch/network/BoostSniffer.hpp"

#include <sys/socket.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>   // ETH_P_IP
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include <vector>
#include <cerrno>

namespace netwatch {

BoostSniffer::BoostSniffer() = default;
BoostSniffer::~BoostSniffer() { stopCapture(); }

void BoostSniffer::startCapture(const std::string& interface) {
    interface_ = interface;
    running_   = true;
    thread_    = std::thread(&BoostSniffer::captureLoop, this);
}

void BoostSniffer::stopCapture() {
    running_ = false;
    if (thread_.joinable()) thread_.join();
}

void BoostSniffer::setPacketHandler(std::function<void(const Packet&)> handler) {
    handler_ = std::move(handler);
}

void BoostSniffer::captureLoop() {
    // 1) Open a packet socket for IPv4
    int sock = ::socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (sock < 0) {
        std::perror("socket(AF_PACKET)");
        return;
    }

    // 2) Bind to the specified interface
    struct ifreq ifr{};
    std::strncpy(ifr.ifr_name, interface_.c_str(), IFNAMSIZ-1);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        std::perror("ioctl(SIOCGIFINDEX)");
        ::close(sock);
        return;
    }
    struct sockaddr_ll sll{};
    sll.sll_family   = AF_PACKET;
    sll.sll_ifindex  = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_IP);
    if (bind(sock, reinterpret_cast<struct sockaddr*>(&sll), sizeof(sll)) < 0) {
        std::perror("bind(AF_PACKET)");
        ::close(sock);
        return;
    }

    // 3) (Optional) Enable promiscuous mode
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
        std::perror("ioctl(SIOCGIFFLAGS)");
    }
    ifr.ifr_flags |= IFF_PROMISC;
    if (ioctl(sock, SIOCSIFFLAGS, &ifr) < 0) {
        std::perror("ioctl(SIOCSIFFLAGS)");
    }

    // 4) Capture loop
    constexpr size_t MAX_PKT = 65536;
    std::vector<uint8_t> buffer(MAX_PKT);
    while (running_) {
        ssize_t len = ::recv(sock, buffer.data(), buffer.size(), 0);
        if (len < 0) {
            if (errno == EINTR) continue;
            std::perror("recv");
            break;
        }

        Packet pkt;
        pkt.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        pkt.data.assign(buffer.begin(), buffer.begin() + len);

        if (handler_) handler_(pkt);
    }

    // 5) Disable promiscuous mode and cleanup
    ifr.ifr_flags &= ~IFF_PROMISC;
    ioctl(sock, SIOCSIFFLAGS, &ifr);
    ::close(sock);
}

} // namespace netwatch
