#include <iostream>
#include <string>
#include "CMD_Parser.h"

int main()
{
    // Testwerte (hart codiert)
    float v_values[] = { 0.1f, 0.0f, -0.2f };
    float w_values[] = { 0.5f, 1.0f, -0.5f };

    const int numTests = 3;

    for (int i = 0; i < numTests; ++i) {
        float* vPtr = &v_values[i];
        float* wPtr = &w_values[i];

        std::string cmd = buildCmdString(vPtr, wPtr);

        std::cout << "Test " << i + 1 << ":\n";
        std::cout << "  v = " << *vPtr << ", w = " << *wPtr << '\n';
        std::cout << "  CMD-String: " << cmd << "\n\n";
    }

    return 0;
}
