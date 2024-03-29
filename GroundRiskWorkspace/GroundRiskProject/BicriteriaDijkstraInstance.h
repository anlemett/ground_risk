#ifndef BICRITERIADIJKSTRAINSTANCE_H
#define BICRITERIADIJKSTRAINSTANCE_H


#include "risks.h"
#include "RiskMap.h"

#include <bits/stdc++.h>
#include <unordered_map>
#include <queue>


class BicriteriaDijkstraInstance
{
public:
    BicriteriaDijkstraInstance(const RiskMap& risk_map, Coord<int> from, Coord<int> to,
                               int search_limit, float r);
    ~BicriteriaDijkstraInstance();
    
    std::vector<Path> computeParetoApxPaths();
    
    Path runWithAlpha(double alpha);
    
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

#endif // BICRITERIADIJKSTRAINSTANCE_H
