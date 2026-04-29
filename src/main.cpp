#include "router/tun_interface.hpp"
#include "router/packet_view.hpp"
#include "router/translation_table.hpp"

#include <iostream>
#include <vector>
#include <array>
#include <arpa/inet.h>

int main() {
    try {
        vrouter::TunInterface tun("vrouter0");
        vrouter::TranslationTable nat_table;
        const uint32_t external_ip = inet_addr("192.168.1.100");

        std::cout << "Router started on " << tun.name() << "\n";
        
        std::array<uint8_t, 2048> buffer;

        while (true) {
            auto n_bytes = tun.receive(buffer);
            if (n_bytes <= 0) continue;

            vrouter::PacketView packet(std::span{buffer.data(), static_cast<size_t>(n_bytes)});

            if (packet.is_valid_ipv4()) {
                uint16_t new_port = nat_table.get_or_create_mapping(packet.src_addr(), packet.src_port());
                
                packet.set_src_addr(external_ip);
                if (packet.protocol() == IPPROTO_TCP || packet.protocol() == IPPROTO_UDP) {
                    packet.set_src_port(new_port);
                }
                
                packet.recalculate_ip_checksum();
                
                std::cout << "Forwarded packet with NAT. New port: " << new_port << "\n";
                tun.send(std::span{buffer.data(), static_cast<size_t>(n_bytes)});
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Router error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}