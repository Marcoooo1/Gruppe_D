#ifndef SHARED_DATA_H
#define SHARED_DATA_H

struct SharedData {
    float scan[360];   // Laserscanner-Werte
    float x;           // Odometrie X (m)
    float y;           // Odometrie Y (m)
    float yaw;         // Orientierung (rad)
};

// globale Variable – Definition kommt in main.cpp
extern SharedData g_shared;

#endif // SHARED_DATA_H
