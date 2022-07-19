#ifndef RISKMAP_H
#define RISKMAP_H

#include "risks.h"
#include "Neighbours.h"

#include <bits/stdc++.h>
#include <utility> //make_pair

class RiskMap
{
public:
    RiskMap();
    ~RiskMap();
    
    int height();
    int width();
    
    float lengthM(Coord<int> p1, Coord<int> p2);
    
    Neighbours neighboursWithin(Coord<int> p, int search_limit);
    
    int risk(Coord<int> p1, Coord<int> p2, float r_m);
    std::pair<Side, Side> parallelogramFromTwoPoints(Coord<int> p1, Coord<int> p2, float r_m, float m_per_pixel);
    int parallelogramRisk(Side orig_side, Side destination_side);
    int riskAt(Coord<int> coord);
    
    std::vector<std::vector<int>> map;
    float m_per_pixel;
    int offset;
};

#endif // RISKMAP_H
