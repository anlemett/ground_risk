#ifndef __TYPES_H__
#define __TYPES_H__

#include "RiskMap.h"
#include <vector>

//template<class T> struct BicriteriaDijkstra {
struct BicriteriaDijkstra {
    RiskMap risk_map;
};

struct Path {
    std::vector<std::vector<int>>  path;
    float linear_combination_weight;
    int risk;
    float length_m;
    float alpha;
};


#endif //__TYPES_H__
