#include <iostream>
#include "LaserParser.h"

int main()
{
    std::size_t arraySize = 0;
    float* data = loadLaserAsFloatArray(arraySize);

    if (!data) {
        std::cerr << "Fehler beim Laden\n";
        return 1;
    }

    // Beispiel-Nutzung
    float* ptr = data;

    // wichtig: löschen
    delete[] data;

    return 0;
}
