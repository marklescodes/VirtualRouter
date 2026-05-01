# Linux Virtual NAT Router (TUN/TAP) 🌐

![C++](https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux-lightgrey.svg)
![Protocol](https://img.shields.io/badge/Protocol-IPv4%20%2F%20TCP-yellow.svg)

A Layer 3 (Network) virtual router implementation that performs **Source NAT (SNAT)**. This project bridges the gap between Linux kernel-space networking and user-space packet manipulation using **TUN devices**, allowing for real-time interception and modification of IP traffic.

## 🚀 Key Features
- **TUN Interface Integration**: Direct interaction with the Linux Kernel via `/dev/net/tun` and `ioctl` syscalls.
- **Stateful NAT Engine**: Implements a connection tracking table to map internal IP/Port pairs to a public IP using `std::unordered_map` with a custom hash.
- **Packet Engineering**:
  - **Manual Checksumming**: Full implementation of the RFC 1071 algorithm to ensure IP header integrity after modification.
  - **Header Manipulation**: Dynamic swapping of IP addresses and Transport-layer ports "on the fly".
- **Zero-Copy Architecture**: Uses lightweight `PacketView` abstractions to process buffers without expensive memory allocations.

## 🛠 Prerequisites

- **OS**: Linux (Kernel must have TUN/TAP support enabled)
- **Compiler**: GCC 9+ or Clang 10+
- **Tools**: iproute2 (for interface management), cmake

## ⚙️ Installation & Build

Clone the repository
```bash
git clone https://github.com/marklescodes/linux-vrouter-nat.git
cd linux-vrouter-nat
```

Build the project
```bash
mkdir build && cd build
cmake ..
make
```
## 🧪 Testing the NAT Logic

To test the router, you need to create a virtual network topology:
**1. Start the Router Node**:
```bash
sudo ./vrouter_node
```

**2. Configure the Interface (In a separate terminal)**:

- Add IP address to the virtual interface
```bash
sudo ip addr add 10.8.0.1/24 dev vrouter0
```
- Bring the interface up
```bash
sudo ip link set dev vrouter0 up
```

**3. Verify Routing:**
- Send a packet through the virtual router
```bash
ping -I vrouter0 1.1.1.1
```
