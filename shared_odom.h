#pragma once

#include <cstddef>

// Daten, die zwischen den Prozessen geteilt werden
struct OdomData {
    float x;
    float y;
    float yaw;
};

// Name und Größe des Shared-Memory-Objekts
inline constexpr const char* ODOM_SHM_NAME = "/odom_shm";
inline constexpr std::size_t ODOM_SHM_SIZE = sizeof(OdomData);

/**
 * Für den Odom-Parser (Writer):
 *  - Shared-Memory-Objekt anlegen (falls nötig) und mappen.
 *
 * @param outPtr  Zeiger auf den gemappten Speicherbereich (OdomData)
 * @param outFd   File-Descriptor des Shared-Memory-Objekts
 * @return true bei Erfolg, sonst false
 */
bool createOdomSharedMemory(OdomData*& outPtr, int& outFd);

/**
 * Für Mapper / Controller (Reader):
 *  - vorhandenes Shared-Memory-Objekt öffnen und mappen.
 *
 * @param outPtr  Zeiger auf den gemappten Speicherbereich (OdomData)
 * @param outFd   File-Descriptor des Shared-Memory-Objekts
 * @return true bei Erfolg, sonst false
 */
bool openOdomSharedMemory(OdomData*& outPtr, int& outFd);

/**
 * Shared Memory wieder freigeben (unmap + close).
 */
void closeOdomSharedMemory(OdomData* ptr, int fd);
