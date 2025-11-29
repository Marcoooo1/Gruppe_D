//Controller Berechnung mit ausgabe als pointer und 4 ziele
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>

// Eigene PI-Konstante
const float PI = 3.14159265358979323846f;

// Konstanten aus Buch
const float K_RHO   = 3.0f;
const float K_ALPHA = 8.0f;
const float K_BETA  = -1.5f;

struct Pose {
    float x;
    float y;
    float z;   // yaw
};

struct Velocity {
    float v;   // lineare Geschwindigkeit
    float w;   // Winkelgeschwindigkeit
};

std::unique_ptr<Velocity> computeVelocity(const Pose& current, const Pose& goal)
{
    auto vel = std::make_unique<Velocity>();

    float dx = goal.x - current.x;
    float dy = goal.y - current.y;
    float dtheta = goal.z - current.z;

    float rho   = sqrtf(dx * dx + dy * dy);
    float alpha = (- dtheta) + atan2f(dy, dx);
    float beta  = (-dtheta) - alpha;

    vel->v = K_RHO * rho;
    vel->w = K_ALPHA * alpha + K_BETA * beta;

    return vel;
}

int main() {
    // Startposition
    Pose current{0.0f, 0.0f, 0.0f};

    // Zielpunkte (4 Stück)
    std::vector<Pose> goals = {
        {1.0f, 1.0f, PI/2.0f},
        {1.5f, 0.5f, PI/4.0f},
        {0.5f, 0.2f, PI},
        {0.2f, 0.3f, 0.0f}  // zurück zum Start
    };

    for (size_t i = 0; i < goals.size(); ++i) {
        std::unique_ptr<Velocity> vel = computeVelocity(current, goals[i]);

        std::cout << "Ziel " << i+1 << ":\n";
        std::cout << "  Linear velocity v:  " << vel->v << " m/s\n";
        std::cout << "  Angular velocity w: " << vel->w << " rad/s\n\n";

        // Aktuelle Position auf das neue Ziel setzen
        current = goals[i];
    }

    return 0;
}

