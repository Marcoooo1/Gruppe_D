#include <fstream>
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


    // Scan verwerfen, wenn ranges nicht genau 360 Werte hat
    if (!scan["ranges"].is_array() || scan["ranges"].size() != 360) {
        return 0; // nichts ausgeben, String/Scan verworfen
    }

    // [angle_increment, range_min, range_max, ranges]
    json out = json::array();
    out.push_back(scan["angle_increment"]);  // kein get<float>(), Wert bleibt wie im Input
    out.push_back(scan["range_min"]);
    out.push_back(scan["range_max"]);
    out.push_back(scan["ranges"]);  

        // Float-Array in eine Datei als JSON-Array schreiben
    std::ofstream("float_array.json") << json(values).dump();

    return 0;
}
