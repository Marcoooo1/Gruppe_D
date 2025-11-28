#ifndef CMD_PARSER
#define CMD_PARSER

#include <cstddef>  // für size_t

// Baut aus v und w einen Command-String.
//
// v, w         : Pointer auf die Geschwindigkeiten
// outBuffer    : Ziel-Buffer (vom Aufrufer bereitgestellt)
// outBufferSize: Größe des Buffers in Bytes
//
// Rückgabewert : true  -> String erfolgreich erstellt
//                false -> Fehler (z.B. Buffer zu klein oder Pointer nullptr)
bool buildCmdString(const float* v,
                    const float* w,
                    char* outBuffer,
                    std::size_t outBufferSize);

#endif // CMD_PARSER_H
