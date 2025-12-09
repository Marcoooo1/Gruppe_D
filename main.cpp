#include <iostream>
#include "shared_data.h"
#include "grid_map.h"
#include "mapper.h"

// globale Variable definieren (Deklaration in shared_data.h)
SharedData g_shared;

int main()
{
    GridMap map;   // eine Map für die ganze Laufzeit
  std::cout << "Map size at runtime = "
          << GridMap::WIDTH << " x " << GridMap::HEIGHT << "\n";

    // ----- SIMULATION: Roboter fährt eine gerade Linie nach rechts -----
    for (int step = 0; step < 100; ++step) {
        // Odometrie simulieren
        g_shared.x   = step * 0.05f;  // 0.05 m pro Schritt -> 5 m Gesamt
        g_shared.y   = 0.0f;
        g_shared.yaw = 0.0f;          // schaut immer nach rechts

        // Laserscan simulieren: kleiner Fächer 2 m vor dem Roboter
        for (int i = 0; i < 360; ++i) {
            // Sichtbereich vorne: -10° bis +10°
            if (i >= 350 || i <= 10)
                g_shared.scan[i] = 2.0f;   // Hindernis 2 m entfernt
            else
                g_shared.scan[i] = 0.0f;   // keine Messung
        }

        // aktuellen Zustand in Map eintragen
        doMappingStep(g_shared, map);
    }

    // Map speichern
    if (map.savePGM("map.pgm")) {
        std::cout << "Map wurde erzeugt: map.pgm\n";
    } else {
        std::cout << "Fehler beim Speichern der Map.\n";
    }

    return 0;
}
