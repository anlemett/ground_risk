#include "Bresenham.h"

Bresenham::Bresenham(float x1, float y1, float x2, float y2)
{
    // Bresenham's line algorithm
    // from https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C.2B.2B
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }
 
    if(x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    
 
        const float dx = x2 - x1;
        const float dy = fabs(y2 - y1);
 
        float error = dx / 2.0f;
        const int ystep = (y1 < y2) ? 1 : -1;
        int y = (int)y1;
    
        const int maxX = (int)x2;
 
        for(int x=(int)x1; x<=maxX; x++) {
            if(steep) {
                //SetPixel(y,x, color);
                std::vector<int> point;
                point.push_back(y);
                point.push_back(x);
                points.push_back(point);
            }
            else {
                //SetPixel(x,y, color);
                std::vector<int> point;
                point.push_back(x);
                point.push_back(y);
                points.push_back(point);
            }
 
            error -= dy;
            if(error < 0) {
                y += ystep;
                error += dx;
            }
        }
    }
}
 
Bresenham::~Bresenham()
{
}


std::vector<std::vector<int>> Bresenham::getPoints()
{
    return points;
}
