#include <iostream>
#include <cmath>

// Structs
struct Pose {
    float x;
    float y;
    float z;   //z = yaw (theta)
};

struct Velocity {
    float v;   // lineare Geschwindigkeit
    float w;   // Winkelgeschwindigkeit
};

//Controller-Funktion
Velocity computeVelocity(const Pose& current, const Pose& goal,
                        float k_rho = 3.0f, float k_alpha = 8.0f, float k_beta = -1.5f,
                        float max_v = 0.5f, float max_w = 2.0f)
{
    Velocity vel{};

    // Deltas
    float dx = goal.x - current.x;
    float dy = goal.y - current.y;
    float dtheta = goal.z - current.z;

    // Rho (Distanz)
    float rho = sqrtf(dx*dx + dy*dy);

    // Alpha (Heading-Fehler)
    float alpha =atan2f(dy, dx) - dtheta;

    // Beta (Orientierungsfehler)
    float beta = dtheta - alpha;

    // Control law
    vel.v = k_rho * rho;
    vel.w = k_alpha * alpha + k_beta * beta;

    // Sättigung

    return vel;
}


int main() {
    // Aktuelle Position hardcodiert
    Pose current;
    current.x = 0.2f;     // z.B. 0.2 Meter
    current.y = 0.3f;
    current.z = 0.0f;     // yaw in radians

    // Zielposition hardcodiert
    Pose goal;
    goal.x = 1.0f;        // z.B. 1 Meter
    goal.y = 1.0f;
    goal.z = 3.1416 / 2;    // Ziel-Yaw = 90°

    // Geschwindigkeit berechnen
    Velocity vel = computeVelocity(current, goal);

    // Ausgabe
    std::cout << "Linear velocity v:  " << vel.v << " m/s\n";
    std::cout << "Angular velocity w: " << vel.w << " rad/s\n";

    return 0;
}

