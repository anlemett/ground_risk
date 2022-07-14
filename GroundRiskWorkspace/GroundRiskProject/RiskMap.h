#ifndef RISKMAP_H
#define RISKMAP_H

#include "risks.h"
#include "NeighboursIter.h"

#include <bits/stdc++.h>

class RiskMap
{
public:
    RiskMap();
    ~RiskMap();
    
    int height();
    int width();
    
    std::vector<Coord> parallelogramFromTwoPoints(Coord p1, Coord p2, float r_m, float m_per_pixel);
    int risk(Coord p1, Coord p2, float r_m);
    float lengthM(Coord p1, Coord p2);
    Neighbours neighboursWithin(Coord p, int search_limit);
    
    std::vector<std::vector<int>> map;
    float m_per_pixel;
    int offset;
};

#endif // RISKMAP_H
