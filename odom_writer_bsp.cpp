// odom_writer.cpp
#include "odom_parser.hpp"
#include "shared_odom.hpp"
#include <iostream>

int main() {
    float values[ODOM_VALUES_COUNT];

    if (!parseOdomFromJson("odom_output.json", values)) {
        return 1;
    }

    OdomData* shmOdom = nullptr;
    int shmFd = -1;

    if (!createOdomSharedMemory(shmOdom, shmFd)) {
        return 1;
    }

    shmOdom->x   = values[0];
    shmOdom->y   = values[1];
    shmOdom->yaw = values[2];

    std::cout << "SHM geschrieben: x=" << shmOdom->x
              << " y=" << shmOdom->y
              << " yaw=" << shmOdom->yaw << '\n';

    closeOdomSharedMemory(shmOdom, shmFd);
    return 0;
}
