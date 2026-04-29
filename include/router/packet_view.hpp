#pragma once

#include <cstdint>
#include <cstddef>

namespace vrouter {

struct BufferView {
    uint8_t* data;
    std::size_t size;
};

class PacketView {
public:
    explicit PacketView(BufferView buffer) : buffer_(buffer) {}

    [[nodiscard]] bool is_valid_ipv4() const;
    [[nodiscard]] uint8_t protocol() const;
    [[nodiscard]] uint32_t src_addr() const;
    [[nodiscard]] uint32_t dst_addr() const;
    void set_src_addr(uint32_t addr);
    
    [[nodiscard]] uint16_t src_port() const;
    void set_src_port(uint16_t port);
    
    void recalculate_ip_checksum();

private:
    BufferView buffer_;
    [[nodiscard]] std::size_t ip_header_len() const;
};

}