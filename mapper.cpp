#define _USE_MATH_DEFINES     // WICHTIG! Damit M_PI verfügbar ist
#include <math.h>
#include <cmath>
#include "mapper.h"
#include <iostream>

void doMappingStep(const SharedData& data, GridMap& map)
{
    const float angle_increment = M_PI / 180.0f; // 1°
    const float angle_start     = -M_PI;         // -180°

    for (int i = 0; i < 360; ++i) {
        float r = data.scan[i];

        if (r <= 0.01f || r > 5.0f) continue;

        float phi = angle_start + i * angle_increment;
        float global_angle = data.yaw + phi;

        float wx = data.x + r * std::cos(global_angle);
        float wy = data.y + r * std::sin(global_angle);

        std::cout << "wx=" << wx << "  wy=" << wy << "\n";

        map.setOccupied(wx, wy);
    }
}
