#include "grid_map.h"
#include <cstring>    // memset
#include <fstream>    // ofstream
#include <iostream>   // cout, cerr

GridMap::GridMap(float res, float ox, float oy)
    : resolution(res), origin_x(ox), origin_y(oy)
{
    clear();
}

void GridMap::clear()
{
    std::memset(cells, 0, sizeof(cells));
}

void GridMap::setOccupied(float wx, float wy)
{
    int mx = static_cast<int>((wx - origin_x) / resolution);
    int my = static_cast<int>((wy - origin_y) / resolution);

    if (mx >= 0 && mx < WIDTH && my >= 0 && my < HEIGHT) {
        // y-Achse nach oben
        cells[HEIGHT - 1 - my][mx] = 255;
    }
}

bool GridMap::savePGM(const std::string& filename) const
{
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Konnte Datei nicht öffnen: " << filename << "\n";
        return false;
    }

    out << "P5\n" << WIDTH << " " << HEIGHT << "\n255\n";
    out.write(reinterpret_cast<const char*>(cells), WIDTH * HEIGHT);
    out.close();
    return true;
}
