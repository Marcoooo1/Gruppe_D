//Controller Berechnung mit ausgabe als pointer
#include <iostream>
#include <cmath>
#include <memory>   // für std::unique_ptr

const float PI = 3.14159265358979323846f;

// Konstanten aus Buch
const float K_RHO   = 3.0f;
const float K_ALPHA = 8.0f;
const float K_BETA  = -1.5f;

struct Pose {
    float x;
    float y;
    float z;   // z = yaw (theta)
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
    float alpha = atan2f(dy, dx) - current.z;
    float beta  = dtheta - alpha;

    vel->v = K_RHO * rho;
    vel->w = K_ALPHA * alpha + K_BETA * beta;

    return vel;
}

int main() {
    Pose current{0.2f, 0.3f, 0.0f};
    Pose goal;

    goal.x = 1.0f;
    goal.y = 1.0f;
    goal.z = PI / 2.0f;   // Kein M_PI, kein <numbers>

    std::unique_ptr<Velocity> vel = computeVelocity(current, goal);

    std::cout << "Linear velocity v:  " << vel->v << " m/s\n";
    std::cout << "Angular velocity w: " << vel->w << " rad/s\n";

    return 0;
}
