#include <iostream>
#include "cmd_parser.h"

int main()
{
    // Testwerte (hart codiert)
    float v_values[] = { 0.1f, 0.0f, -0.2f };
    float w_values[] = { 0.5f, 1.0f, -0.5f };

    // Buffer für den Ausgabe-String
    char cmdBuffer[128];  // 128 Bytes reichen hier locker

    const int numTests = 3;

    for (int i = 0; i < numTests; ++i) {
        float* vPtr = &v_values[i];
        float* wPtr = &w_values[i];

        bool ok = buildCmdString(vPtr, wPtr, cmdBuffer, sizeof(cmdBuffer));

        std::cout << "Test " << i + 1 << ":\n";
        std::cout << "  v = " << *vPtr << ", w = " << *wPtr << '\n';

        if (ok) {
            std::cout << "  CMD-String: " << cmdBuffer << "\n\n";
        } else {
            std::cout << "  Fehler: CMD-String konnte nicht erzeugt werden!\n\n";
        }
    }

    return 0;
}
