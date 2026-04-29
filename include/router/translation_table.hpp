#pragma once

#include <unordered_map>
#include <cstdint>

namespace vrouter {

struct ConnectionKey {
    uint32_t internal_ip;
    uint16_t internal_port;

    bool operator==(const ConnectionKey& other) const {
        return internal_ip == other.internal_ip && internal_port == other.internal_port;
    }
};

struct ConnectionHash {
    std::size_t operator()(const ConnectionKey& k) const {
        return std::hash<uint32_t>{}(k.internal_ip) ^ (std::hash<uint16_t>{}(k.internal_port) << 1);
    }
};

class TranslationTable {
public:
    uint16_t get_or_create_mapping(uint32_t ip, uint16_t port) {
        ConnectionKey key{ip, port};
        if (auto it = internal_to_external_.find(key); it != internal_to_external_.end()) {
            return it->second;
        }

        uint16_t new_port = current_port_++;
        internal_to_external_[key] = new_port;
        return new_port;
    }

private:
    std::unordered_map<ConnectionKey, uint16_t, ConnectionHash> internal_to_external_;
    uint16_t current_port_{49152}; 
};

}