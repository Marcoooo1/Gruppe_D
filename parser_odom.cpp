#include "parser_odom.h"

#include <fstream>
#include <string>
#include <cmath>
#include <iostream>

#include "json.hpp"   // nlohmann/json

using json = nlohmann::json;

bool parseOdomFromJson(const std::string& filename, float* outValues)
{
    if (!outValues) {
        return false;
    }

    // JSON-Datei komplett in einen String lesen
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Kann Datei nicht öffnen: " << filename << '\n';
        return false;
    }

    std::string jsonStr((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());

    // JSON parsen
    json j = json::parse(jsonStr);

    // Falls Datei ein Array von Odom-Messages enthält -> ersten nehmen
    json odom = j.is_array() ? j[0] : j;

    // Position
    double px = odom["pose"]["pose"]["position"]["x"].get<double>();
    double py = odom["pose"]["pose"]["position"]["y"].get<double>();

    // Orientierung (Quaternion)
    double qx = odom["pose"]["pose"]["orientation"]["x"].get<double>();
    double qy = odom["pose"]["pose"]["orientation"]["y"].get<double>();
    double qz = odom["pose"]["pose"]["orientation"]["z"].get<double>();
    double qw = odom["pose"]["pose"]["orientation"]["w"].get<double>();

    // Yaw aus Quaternion berechnen (Rotation um z-Achse)
    double yaw = std::atan2(
        2.0 * (qw * qz + qx * qy),
        1.0 - 2.0 * (qy * qy + qz * qz)
    );

    // Array füllen: [x, y, yaw]
    outValues[0] = static_cast<float>(px);
    outValues[1] = static_cast<float>(py);
    outValues[2] = static_cast<float>(yaw);

    // Pointer auf das Array (zur Demonstration, gleiche Werte wie outValues)
    float* ptr = outValues;

    // Ausgabe im Output (Konsole)
    std::cout << "x="   << ptr[0]
              << " y="   << ptr[1]
              << " yaw=" << ptr[2] << '\n';

    return true;
}
