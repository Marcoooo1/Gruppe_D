#include <iostream>
#include <vector>
#include "controller.h"

int main() {
    Pose current{0.0f, 0.0f, 0.0f};

    std::vector<Pose> goals = {
        {1.0f, 1.0f, PI/2.0f},
        {1.5f, 0.5f, PI/4.0f},
        {0.5f, 0.2f, PI},
        {0.2f, 0.3f, 0.0f}
    };

    for (size_t i = 0; i < goals.size(); ++i) {
        auto vel = computeVelocity(current, goals[i]);

        std::cout << "Ziel " << i+1 << ":\n";
        std::cout << "  Linear velocity v:  " << vel->v << " m/s\n";
        std::cout << "  Angular velocity w: " << vel->w << " rad/s\n\n";

        current = goals[i];
    }

    return 0;
}
