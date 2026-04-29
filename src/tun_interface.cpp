#include "router/tun_interface.hpp"

#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

namespace vrouter {

TunInterface::TunInterface(std::string_view name) : name_(name) {
    fd_ = open("/dev/net/tun", O_RDWR);
    if (fd_ < 0) throw std::runtime_error("Cannot open /dev/net/tun");

    ifreq ifr{};
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    std::strncpy(ifr.ifr_name, name_.c_str(), IFNAMSIZ);

    if (ioctl(fd_, TUNSETIFF, &ifr) < 0) {
        close(fd_);
        throw std::runtime_error("ioctl(TUNSETIFF) failed for " + name_);
    }
}

TunInterface::~TunInterface() {
    if (fd_ >= 0) close(fd_);
}

std::ssize_t TunInterface::receive(std::span<uint8_t> buffer) const {
    return read(fd_, buffer.data(), buffer.size());
}

std::ssize_t TunInterface::send(std::span<const uint8_t> packet) const {
    return write(fd_, packet.data(), packet.size());
}

}