#ifndef BICRITERIADIJKSTRAINSTANCE_H
#define BICRITERIADIJKSTRAINSTANCE_H

#include <utm50_uavis2/RiskAwareRisks.hh>
#include <utm50_uavis2/RiskAwareRiskMap.hh>

class BicriteriaDijkstraInstance
{
public:
    BicriteriaDijkstraInstance(const RiskMap& risk_map, Coord<int> from, Coord<int> to,
                               int search_limit, float r, int offset);
    ~BicriteriaDijkstraInstance();
    
    std::vector<Path> computeParetoApxPaths();

private:
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
    int offset;
};

#endif // BICRITERIADIJKSTRAINSTANCE_H
