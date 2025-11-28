#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include <string>

// Erzeugt aus v und w einen Command-String im Format:
// ---START---{"linear": v, "angular": w}___END___
//
// v, w : Pointer auf die Geschwindigkeiten
// Rückgabe: fertiger String
std::string buildCmdString(const float* v, const float* w);

#endif // CMD_PARSER_H
