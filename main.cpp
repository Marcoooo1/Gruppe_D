
#include <iostream>
#include "shared_data.h"
#include "grid_map.h"
#include "mapper.h"

int main()
{
    SharedData shared{};

    // Beispielpose vom Roboter
    shared.x = 0.0f;
    shared.y = 0.0f;
    shared.yaw = 0.0f;   // Richtung „nach rechts“ im Bild

    // Beispiel-Laserscan: 2 m in alle Richtungen
    for (int i = 0; i < 360; ++i) {
        shared.scan[i] = 2.0f;
    }

    GridMap map;            // 20 m x 20 m, 5 cm Auflösung
    doMappingStep(shared, map);

    if (map.savePGM("map.pgm")) {
        std::cout << "Map wurde erzeugt: map.pgm\n";
    } else {
        std::cout << "Fehler beim Speichern der Map.\n";
    }

    return 0;
}
