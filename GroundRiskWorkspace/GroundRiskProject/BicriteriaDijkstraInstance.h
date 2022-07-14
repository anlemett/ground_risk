#ifndef BICRITERIADIJKSTRAINSTANCE_H
#define BICRITERIADIJKSTRAINSTANCE_H


#include "risks.h"
#include "RiskMap.h"

#include <bits/stdc++.h>
#include <unordered_map>
#include <queue>

//template<class T> struct BicriteriaDijkstra {
struct BicriteriaDijkstra {
    RiskMap risk_map;
};

class BicriteriaDijkstraInstance
{
public:
    BicriteriaDijkstraInstance(RiskMap& risk_map, Coord from, Coord to,
                               int search_limit, float r);
    ~BicriteriaDijkstraInstance();
    
    std::vector<Path> computeParetoApxPaths();
    
    Path runWithAlpha(float alpha);
    
    Path unwrapPath(std::unordered_map<Coord, Coord, hash_fn> nodes_previous,
                    std::unordered_map<Coord, float, hash_fn> nodes_labels,
                    float alpha);

private:
    RiskMap risk_map;
    Coord from;
    Coord to;
    int search_limit;
    float r_m;
};

#endif // BICRITERIADIJKSTRAINSTANCE_H
