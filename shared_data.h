

#ifndef SHARED_DATA_H
#define SHARED_DATA_H

struct SharedData {
    float scan[360];   // 360 Laserwerte (Meter)
    float x;           // Roboterposition (m)
    float y;
    float yaw;         // Orientierung (rad)
};

#endif // SHARED_DATA_H
