#ifndef GRID_MAP_H_INCLUDED
#define GRID_MAP_H_INCLUDED



#endif // GRID_MAP_H_INCLUDED
#ifndef GRID_MAP_H
#define GRID_MAP_H

#include <string>

struct GridMap {
    static const int WIDTH  = 400;
    static const int HEIGHT = 400;

    float resolution;   // m pro Zelle
    float origin_x;     // Weltkoordinate von Zelle (0,0)
    float origin_y;

    unsigned char cells[HEIGHT][WIDTH];

    GridMap(float res = 0.05f, float ox = -10.0f, float oy = -10.0f);

    void clear();
    void setOccupied(float wx, float wy);
    bool savePGM(const std::string& filename) const;
};

#endif // GRID_MAP_H
