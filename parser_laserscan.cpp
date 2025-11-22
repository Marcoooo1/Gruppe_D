#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

// nlohmann/json (https://github.com/nlohmann/json)
// json.hpp muss im Include-Pfad liegen
#include "json.hpp"

using json = nlohmann::json;

// Struktur für einen vorbereiteten Scan
struct ScanData {
    float angle_increment{};
    float range_min{};
    float range_max{};
    std::vector<float> ranges;  // alle Distanzwerte

    // Alles (inkl. angle_increment, range_min, range_max) in EIN Float-Array packen
    std::vector<float> toFloatArray() const {
        std::vector<float> result;
        result.reserve(3 + ranges.size()); // 3 Header-Werte + alle Ranges

        result.push_back(angle_increment);
        result.push_back(range_min);
        result.push_back(range_max);

        result.insert(result.end(), ranges.begin(), ranges.end());
        return result;
    }
};

// JSON-Datei einlesen und alle Scans parsen
std::vector<ScanData> parseLaserScans(const std::string &filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Konnte Datei nicht öffnen: " + filename);
    }

    json j;
    in >> j;  // ganze Datei als JSON parsen

    std::vector<ScanData> scans;

    if (j.is_array()) {
        // Mehrere Scans im Array
        for (const auto &scanJson : j) {
            if (!scanJson.contains("angle_increment") ||
                !scanJson.contains("range_min") ||
                !scanJson.contains("range_max") ||
                !scanJson.contains("ranges")) {
                // Falls ein Objekt unvollständig ist -> überspringen
                continue;
            }

            ScanData s;
            s.angle_increment = scanJson.at("angle_increment").get<float>();
            s.range_min       = scanJson.at("range_min").get<float>();
            s.range_max       = scanJson.at("range_max").get<float>();

            // ranges[] als float-Vektor übernehmen
            for (const auto &val : scanJson.at("ranges")) {
                s.ranges.push_back(val.get<float>());
            }

            scans.push_back(std::move(s));
        }
    } else if (j.is_object()) {
        // Fall: Datei enthält nur einen Scan
        ScanData s;
        s.angle_increment = j.at("angle_increment").get<float>();
        s.range_min       = j.at("range_min").get<float>();
        s.range_max       = j.at("range_max").get<float>();

        for (const auto &val : j.at("ranges")) {
            s.ranges.push_back(val.get<float>());
        }

        scans.push_back(std::move(s));
    } else {
        throw std::runtime_error("Unerwartetes JSON-Format in Datei: " + filename);
    }

    return scans;
}

int main() {
    try {
        // Datei mit deinen LaserScan-Werten
        const std::string filename = "laser_output.json";

        // Alle Scans parsen
        std::vector<ScanData> scans = parseLaserScans(filename);

        std::cout << "Anzahl geladener Scans: " << scans.size() << '\n';

        if (!scans.empty()) {
            const ScanData &first = scans.front();

            // Beispiel: ersten Scan in ein Float-Array umwandeln
            std::vector<float> arr = first.toFloatArray();

            std::cout << "Erster Scan:\n";
            std::cout << "  angle_increment = " << first.angle_increment << '\n';
            std::cout << "  range_min       = " << first.range_min       << '\n';
            std::cout << "  range_max       = " << first.range_max       << '\n';
            std::cout << "  Anzahl ranges   = " << first.ranges.size()   << '\n';
            std::cout << "  Länge Float-Array (inkl. 3 Header-Werte) = "
                      << arr.size() << '\n';

            // Wenn du ein echtes C-Array brauchst:
            // float *raw = arr.data();
            // size_t len = arr.size();
        }

    } catch (const std::exception &e) {
        std::cerr << "Fehler: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
