#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"        // nlohmann/json

using json = nlohmann::json;

//Datei → String → JSON → Float-Array.

int main() {
    // JSON-Datei komplett in einen String lesen
    std::ifstream in("laser_output.json");
    std::string jsonStr((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());

    // JSON parsen
    json j = json::parse(jsonStr);

    // Falls Datei ein Array von Scans enthält -> ersten nehmen
    json scan = j.is_array() ? j[0] : j;

    // Scan verwerfen, wenn ranges nicht genau 360 Werte hat
    if (!scan["ranges"].is_array() || scan["ranges"].size() != 360) {
        return 0; // nichts ausgeben, String/Scan verworfen
    }

    // Float-Array aufbauen: [angle_increment, range_min, range_max, ranges...]
    std::vector<float> values;
    values.reserve(3 + scan["ranges"].size());

    values.push_back(scan["angle_increment"].get<float>());
    values.push_back(scan["range_min"].get<float>());
    values.push_back(scan["range_max"].get<float>());

    for (auto &v : scan["ranges"]) {
        values.push_back(v.get<float>());
    }

        // Float-Array in eine Datei als JSON-Array schreiben
    std::ofstream("float_array.json") << json(values).dump();

    return 0;
}
