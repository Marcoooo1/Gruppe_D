#include "CMD_Parser.h"
#include <string>

std::string buildCmdString(const float* v, const float* w)
{
    // einfache Sicherheitschecks
    if (v == nullptr || w == nullptr) {
        return "---START---{\"linear\": 0.0, \"angular\": 0.0}___END___";
    }

    // sehr simple Implementierung mit std::to_string
    // (wenn du später weniger Nachkommastellen willst, kann man das noch formatieren)
    std::string result = "---START---{\"linear\": ";
    result += std::to_string(*v);
    result += ", \"angular\": ";
    result += std::to_string(*w);
    result += "}___END___";

    return result;
}
