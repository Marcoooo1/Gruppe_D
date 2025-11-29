// mapper.cpp   (für controller.cpp identisch vom Prinzip)
#include "shared_odom.hpp"
#include <iostream>

int main() {
    OdomData* shmOdom = nullptr;
    int shmFd = -1;

    if (!openOdomSharedMemory(shmOdom, shmFd)) {
        return 1;
    }

    std::cout << "SHM gelesen: x=" << shmOdom->x
              << " y=" << shmOdom->y
              << " yaw=" << shmOdom->yaw << '\n';

    // hier weiterverarbeiten ...

    closeOdomSharedMemory(shmOdom, shmFd);
    return 0;
}
