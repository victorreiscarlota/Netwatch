#include "netwatch/parser/PacketParser.hpp"
#include <cstring>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

namespace netwatch {

std::optional<ParsedPacket> PacketParser::parse(const Packet& pkt) {
    const auto* data = pkt.data.data();
    size_t len = pkt.data.size();
    if (len < 14 + sizeof(struct iphdr)) return std::nullopt;

    const uint8_t* ip_frame = data + 14;
    size_t ip_len = len - 14;

    auto* iph = (struct iphdr*)ip_frame;
    if (iph->version != 4) return std::nullopt;
    size_t ip_header_len = iph->ihl * 4;
    if (ip_len < ip_header_len) return std::nullopt;

    ParsedPacket out;
    out.timestamp = pkt.timestamp;
    char src[INET_ADDRSTRLEN], dst[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &iph->saddr, src, sizeof(src));
    inet_ntop(AF_INET, &iph->daddr, dst, sizeof(dst));
    out.src_ip = src;
    out.dst_ip = dst;

    const uint8_t* transport = ip_frame + ip_header_len;
    size_t transport_len = ip_len - ip_header_len;

    if (iph->protocol == IPPROTO_TCP && transport_len >= sizeof(struct tcphdr)) {
        auto* tcph = (struct tcphdr*)transport;
        out.src_port = ntohs(tcph->source);
        out.dst_port = ntohs(tcph->dest);
        out.protocol = "TCP";
        size_t total_len = ntohs(iph->tot_len);
        out.size = total_len - ip_header_len - (tcph->doff * 4);
    }
    else if (iph->protocol == IPPROTO_UDP && transport_len >= sizeof(struct udphdr)) {
        auto* udph = (struct udphdr*)transport;
        out.src_port = ntohs(udph->source);
        out.dst_port = ntohs(udph->dest);
        out.protocol = "UDP";
        out.size = ntohs(udph->len) - sizeof(struct udphdr);
    }
    else {
        return std::nullopt;
    }

    return out;
}

} // namespace netwatch
