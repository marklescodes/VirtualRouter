#include "router/packet_view.hpp"
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

namespace vrouter {

bool PacketView::is_valid_ipv4() const {
    if (data_.size() < sizeof(iphdr)) return false;
    const auto* ip = reinterpret_cast<const iphdr*>(data_.data());
    return ip->version == 4;
}

uint8_t PacketView::protocol() const {
    return reinterpret_cast<const iphdr*>(data_.data())->protocol;
}

uint32_t PacketView::src_addr() const {
    return reinterpret_cast<const iphdr*>(data_.data())->saddr;
}

uint32_t PacketView::dst_addr() const {
    return reinterpret_cast<const iphdr*>(data_.data())->daddr;
}

void PacketView::set_src_addr(uint32_t addr) {
    reinterpret_cast<iphdr*>(data_.data())->saddr = addr;
}

size_t PacketView::ip_header_len() const {
    return reinterpret_cast<const iphdr*>(data_.data())->ihl * 4;
}

uint16_t PacketView::src_port() const {
    const size_t offset = ip_header_len();
    return ntohs(*reinterpret_cast<const uint16_t*>(data_.data() + offset));
}

void PacketView::set_src_port(uint16_t port) {
    const size_t offset = ip_header_len();
    *reinterpret_cast<uint16_t*>(data_.data() + offset) = htons(port);
}

void PacketView::recalculate_ip_checksum() {
    auto* ip = reinterpret_cast<iphdr*>(data_.data());
    ip->check = 0;
    uint32_t sum = 0;
    const uint16_t* ptr = reinterpret_cast<const uint16_t*>(ip);
    for (int i = 0; i < (ip->ihl * 2); ++i) sum += ptr[i];
    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    ip->check = static_cast<uint16_t>(~sum);
}

}