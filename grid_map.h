#ifndef GRID_MAP_H
#define GRID_MAP_H

#include <string>

struct GridMap {
    static const int WIDTH  = 100;
    static const int HEIGHT = 100;

    float resolution;   // m pro Zelle
    float origin_x;     // Weltkoordinate von Zelle (0,0)
    float origin_y;

    unsigned char cells[HEIGHT][WIDTH];

    GridMap(float res = 0.03f, float ox = -3.0f, float oy = -3.0f);

    void clear();
    void setOccupied(float wx, float wy);
    bool savePGM(const std::string& filename) const;
};

#endif // GRID_MAP_H
