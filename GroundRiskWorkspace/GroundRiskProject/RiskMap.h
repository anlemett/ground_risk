#ifndef RISKMAP_H
#define RISKMAP_H

#include "NeighboursIter.h"

#include <bits/stdc++.h>

class RiskMap
{
public:
    RiskMap();
    ~RiskMap();
    
    int height();
    int width();
    NeighboursIter neighboursWithin(std::vector<int> p, int search_limit);
    
    std::vector<std::vector<int>> map;
    float m_per_pixel;
    int offset;
};

#endif // RISKMAP_H
