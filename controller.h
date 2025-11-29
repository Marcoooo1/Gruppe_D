#pragma once
#include <memory>

// Konstanten
constexpr float PI = 3.14159265358979323846f;

constexpr float K_RHO   = 3.0f;
constexpr float K_ALPHA = 8.0f;
constexpr float K_BETA  = -1.5f;

constexpr float MAX_V = 0.25f;   // maximale lineare Geschwindigkeit [m/s]
constexpr float MAX_W = 1.80f;   // maximale Winkelgeschwindigkeit [rad/s]

// Datenstrukturen
struct Pose {
    float x;
    float y;
    float z;   // yaw
};

struct Velocity {
    float v;   // lineare Geschwindigkeit
    float w;   // Winkelgeschwindigkeit
};

// Funktionen
float clamp(float value, float minVal, float maxVal);
std::unique_ptr<Velocity> computeVelocity(const Pose& current, const Pose& goal);
