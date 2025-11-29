#include "controller.h"
#include <cmath>
#include <memory>

float clamp(float value, float minVal, float maxVal) {
    if (value > maxVal) return maxVal;
    if (value < minVal) return minVal;
    return value;
}

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

    vel->v = clamp(vel->v, -MAX_V, MAX_V);
    vel->w = clamp(vel->w, -MAX_W, MAX_W);

    return vel;
}
