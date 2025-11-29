#pragma once
#include <cstddef>

// Liefert Pointer auf Float-Array.
// outSize = Größe des Arrays (3 + ranges.size).
// Rückgabe: float* (muss vom Benutzer mit delete[] freigegeben werden).
float* loadLaserAsFloatArray(std::size_t& outSize);
