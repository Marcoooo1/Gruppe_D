#include "cmd_parser.h"
#include <cstdio>   // für std::snprintf

bool buildCmdString(const float* v,
                    const float* w,
                    char* outBuffer,
                    std::size_t outBufferSize)
{
    // Einfache Sicherheitschecks auf gültige Pointer und Buffergröße
    if (v == nullptr || w == nullptr || outBuffer == nullptr || outBufferSize == 0) {
        return false;
    }

    // String im gewünschten Format:
    // ---START---{"linear": 0.1, "angular": 0.5}___END___
    //
    // %.3f -> 3 Nachkommastellen; kannst du bei Bedarf ändern
    int written = std::snprintf(
        outBuffer,
        outBufferSize,
        "---START---{\"linear\": %.3f, \"angular\": %.3f}___END___",
        *v,
        *w
    );

    // snprintf gibt die Anzahl der Zeichen zurück, die geschrieben werden
    // würden (ohne Nullterminator). Wenn das >= Buffergröße ist, war der
    // Buffer zu klein.
    if (written < 0 || static_cast<std::size_t>(written) >= outBufferSize) {
        return false;
    }

    return true;
}
