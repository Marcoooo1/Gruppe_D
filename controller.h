#pragma once
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>

// Eigene PI-Konstante
constexpr float PI = 3.14159265358979323846f;

// Konstanten
constexpr float K_RHO   = 3.0f;
constexpr float K_ALPHA = 8.0f;
constexpr float K_BETA  = -1.5f;

// Maximale Geschwindigkeiten
constexpr float MAX_V = 0.25f;   // [m/s]
constexpr float MAX_W = 1.80f;   // [rad/s]

// Pose-Struktur
struct Pose {
    float x;
    float y;
    float z; // yaw
};

// Velocity-Struktur
struct Velocity {
    float v;
    float w;
};

// Hilfsfunktion zum Begrenzen
inline float clamp(float value, float minVal, float maxVal) {
    if (value > maxVal) return maxVal;
    if (value < minVal) return minVal;
    return value;
}

// Berechnung der Geschwindigkeit
inline std::unique_ptr<Velocity> computeVelocity(const Pose& current, const Pose& goal) {
    auto vel = std::make_unique<Velocity>();

    float dx = goal.x - current.x;
    float dy = goal.y - current.y;
    float dtheta = goal.z - current.z;

    float rho   = std::sqrt(dx * dx + dy * dy);
    float alpha = (-dtheta) + std::atan2(dy, dx);
    float beta  = (-dtheta) - alpha;

    vel->v = K_RHO * rho;
    vel->w = K_ALPHA * alpha + K_BETA * beta;

    vel->v = clamp(vel->v, -MAX_V, MAX_V);
    vel->w = clamp(vel->w, -MAX_W, MAX_W);

    return vel;
}

// Funktion, die alles ausführt (wie dein main)
inline void runController() {
    Pose current{0.0f, 0.0f, 0.0f};

    std::vector<Pose> goals = {
        {1.0f, 1.0f, PI/2.0f},
        {1.5f, 0.5f, PI/4.0f},
        {0.5f, 0.2f, PI},
        {0.2f, 0.3f, 0.0f}  // zurück zum Start
    };

    for (size_t i = 0; i < goals.size(); ++i) {
        auto vel = computeVelocity(current, goals[i]);

        std::cout << "Ziel " << i+1 << ":\n";
        std::cout << "  Linear velocity v:  " << vel->v << " m/s\n";
        std::cout << "  Angular velocity w: " << vel->w << " rad/s\n\n";

        current = goals[i];
    }
}
