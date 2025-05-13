#include "netwatch/network/BoostSniffer.hpp"
#include "netwatch/storage/SQLiteStorage.hpp"
#include "netwatch/parser/PacketParser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    const std::string iface = (argc > 1 ? argv[1] : "eth0");
    const std::string dbPath = (argc > 2 ? argv[2] : "packets.db");

    netwatch::SQLiteStorage storage(dbPath);
    netwatch::BoostSniffer sniffer;
    netwatch::PacketParser parser;

    sniffer.setPacketHandler([&](const netwatch::Packet& pkt) {
        auto parsed = parser.parse(pkt);
        if (parsed) {
            std::cout << parsed->protocol << " "
                      << parsed->src_ip << ":" << parsed->src_port
                      << " -> " << parsed->dst_ip << ":" << parsed->dst_port
                      << " size=" << parsed->size << "\n";
            storage.persistPacket(*parsed);
        }
    });

    std::cout << "Starting capture on " << iface << "\n";
    sniffer.startCapture(iface);
    std::cout << "Press ENTER to stop...\n";
    std::cin.get();
    sniffer.stopCapture();
    return 0;
}
