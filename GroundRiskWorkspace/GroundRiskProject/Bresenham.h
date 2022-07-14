#ifndef BRESENHAM_H
#define BRESENHAM_H

#include <vector>
#include <math.h>
#include "risks.h"

class Bresenham
{
public:
    Bresenham(float x1, float y1, float x2, float y2);
    ~Bresenham();
    
    std::vector<Coord> getPoints();
    
private:
    std::vector<Coord> points;
};

#endif // BRESENHAM_H
