#pragma once

#include <string>
#include <string_view>
#include <cstddef>
#include <cstdint>

namespace vrouter {

class TunInterface {
public:
    explicit TunInterface(std::string_view name);
    ~TunInterface();

    TunInterface(const TunInterface&) = delete;
    TunInterface& operator=(const TunInterface&) = delete;

    std::ptrdiff_t send(const uint8_t* data, std::size_t size) const;
    std::ptrdiff_t receive(uint8_t* buffer, std::size_t size) const;

    [[nodiscard]] std::string_view name() const noexcept { return name_; }

private:
    int fd_{-1};
    std::string name_;
};

}