cmake_minimum_required(VERSION 3.15)
project(VirtualRouterEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(router_core
    src/tun_interface.cpp
    src/packet_view.cpp
)

target_include_directories(router_core PUBLIC include)

add_executable(vrouter_node src/main.cpp)
target_link_libraries(vrouter_node PRIVATE router_core)