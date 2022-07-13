#ifndef BICRITERIADIJKSTRAINSTANCE_H
#define BICRITERIADIJKSTRAINSTANCE_H


#include "types.h"

#include <bits/stdc++.h>
#include <unordered_map>
#include <queue>

class BicriteriaDijkstraInstance
{
public:
    BicriteriaDijkstraInstance(RiskMap& risk_map, std::vector<int> from, std::vector<int> to,
                               int search_limit, float r);
    ~BicriteriaDijkstraInstance();
    
    std::vector<Path> computeParetoApxPaths();
    
    Path runWithAlpha(float alpha);
    
    Path unwrapPath(std::unordered_map<std::vector<int>, std::vector<int>> nodes_previous,
                    std::unordered_map<std::vector<int>, int> nodes_labels,
                    float alpha);

private:
    RiskMap risk_map;
    std::vector<int> from;
    std::vector<int> to;
    int search_limit;
    float r_m;
};

#endif // BICRITERIADIJKSTRAINSTANCE_H
