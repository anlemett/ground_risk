#ifndef ASTARSEARCHINSTANCE_H
#define ASTARSEARCHINSTANCE_H

#include "risks.h"
#include "RiskMap.h"
#include "Neighbours.h"

#include <bits/stdc++.h>
#include <unordered_map>
#include <queue>


class AStarSearchInstance
{
public:
    AStarSearchInstance(const RiskMap& risk_map, Coord<int> from, Coord<int> to,
    int search_limit, float r);
    ~AStarSearchInstance();
    
    std::vector<Path> computeParetoApxPaths();
    
    Path runWithAlpha(double alpha);
    
    double h(Coord<int> p);
    
    Path unwrapPath(const std::unordered_map<Coord<int>, Coord<int>, hash_fn>& nodes_previous,
                    const std::unordered_map<Coord<int>, double, hash_fn>& nodes_labels,
                    double alpha);

private:
    RiskMap risk_map;
    Coord<int> from;
    Coord<int> to;
    int search_limit;
    float r_m;
};

#endif // ASTARSEARCHINSTANCE_H
