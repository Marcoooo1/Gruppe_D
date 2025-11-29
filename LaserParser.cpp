#include "LaserParser.h"

#include <fstream>
#include <string>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

float* loadLaserAsFloatArray(std::size_t& outSize)
{
    // Datei einlesen
    std::ifstream in("laser_output.json");
    if (!in) {
        std::cerr << "Kann laser_output.json nicht öffnen\n";
        return nullptr;
    }

    std::string jsonStr((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());

    json j = json::parse(jsonStr);
    json scan = j.is_array() ? j[0] : j;

    if (!scan.contains("ranges") || !scan["ranges"].is_array()) {
        std::cerr << "\"ranges\" fehlt oder ist kein Array\n";
        return nullptr;
    }

    std::size_t numRanges = scan["ranges"].size();

    // Gesamtgröße: 3 Header-Werte + ranges[]
    outSize = 3 + numRanges;

    // Speicher anlegen
    float* arr = new float[outSize];

    // Header
    arr[0] = scan["angle_increment"].get<float>();
    arr[1] = scan["range_min"].get<float>();
    arr[2] = scan["range_max"].get<float>();

    // ranges kopieren
    for (std::size_t i = 0; i < numRanges; ++i) {
        arr[3 + i] = scan["ranges"][i].get<float>();
    }

    return arr;
}
