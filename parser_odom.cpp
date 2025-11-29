#include <fstream>
#include <string>
#include <cstddef>
#include <cmath>
#include "json.hpp"     // nlohmann/json

using json = nlohmann::json;

int main() {
    // JSON-Datei komplett in einen String lesen
    std::ifstream in("odom_output.json");   // Dateiname bei Bedarf anpassen
    if (!in) return 1;

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

    // Array mit [x, y, yaw]
    constexpr std::size_t NUM_VALUES = 3;
    static float values[NUM_VALUES];

    values[0] = static_cast<float>(px);
    values[1] = static_cast<float>(py);
    values[2] = static_cast<float>(yaw);

    // Pointer auf das Array
    float* ptr = values;

      float* ptr = values;

    // Array über Pointer im Output ausgeben
    std::cout << "x="   << ptr[0]
              << " y="   << ptr[1]
              << " yaw=" << ptr[2] << '\n';


    return 0;
}
