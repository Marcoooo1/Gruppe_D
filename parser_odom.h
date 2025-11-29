#pragma once

#include <cstddef>
#include <string>

// Anzahl der Werte im Ausgabearray: [x, y, yaw]
constexpr std::size_t ODOM_VALUES_COUNT = 3;
bool parseOdomFromJson(const std::string& filename, float* outValues);
