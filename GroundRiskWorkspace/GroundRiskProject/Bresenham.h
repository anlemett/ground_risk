#ifndef BRESENHAM_H
#define BRESENHAM_H

#include <vector>
#include <math.h> 

class Bresenham
{
public:
    Bresenham(float x1, float y1, float x2, float y2);
    ~Bresenham();
    
    std::vector<std::vector<int>> getPoints();
    
private:
    std::vector<std::vector<int>> points;
};

#endif // BRESENHAM_H
