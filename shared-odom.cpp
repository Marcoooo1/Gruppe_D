#include "shared_odom.hpp"

#include <fcntl.h>    // O_CREAT, O_RDWR
#include <sys/mman.h> // shm_open, mmap, PROT_*, MAP_*
#include <unistd.h>   // ftruncate, close
#include <iostream>   // perror / cerr

bool createOdomSharedMemory(OdomData*& outPtr, int& outFd)
{
    outPtr = nullptr;
    outFd  = -1;

    int fd = shm_open(ODOM_SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        std::perror("shm_open (create)");
        return false;
    }

    if (ftruncate(fd, ODOM_SHM_SIZE) == -1) {
        std::perror("ftruncate");
        close(fd);
        return false;
    }

    void* addr = mmap(nullptr, ODOM_SHM_SIZE,
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        std::perror("mmap (create)");
        close(fd);
        return false;
    }

    outPtr = static_cast<OdomData*>(addr);
    outFd  = fd;
    return true;
}

bool openOdomSharedMemory(OdomData*& outPtr, int& outFd)
{
    outPtr = nullptr;
    outFd  = -1;

    int fd = shm_open(ODOM_SHM_NAME, O_RDWR, 0666);
    if (fd == -1) {
        std::perror("shm_open (open)");
        return false;
    }

    void* addr = mmap(nullptr, ODOM_SHM_SIZE,
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        std::perror("mmap (open)");
        close(fd);
        return false;
    }

    outPtr = static_cast<OdomData*>(addr);
    outFd  = fd;
    return true;
}

void closeOdomSharedMemory(OdomData* ptr, int fd)
{
    if (ptr) {
        munmap(static_cast<void*>(ptr), ODOM_SHM_SIZE);
    }
    if (fd != -1) {
        close(fd);
    }
}
