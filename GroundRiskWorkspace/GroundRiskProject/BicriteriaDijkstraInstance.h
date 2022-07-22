#ifndef BICRITERIADIJKSTRAINSTANCE_H
#define BICRITERIADIJKSTRAINSTANCE_H


#include "risks.h"
#include "RiskMap.h"

#include <bits/stdc++.h>
#include <unordered_map>
#include <queue>

struct comparator {
        bool operator()(
            const std::pair<Coord<int>, float>& a,
            const std::pair<Coord<int>, float>& b)
            {
                return a.second > b.second;
            }
        };

//https://stackoverflow.com/questions/19467485/how-to-remove-element-not-at-top-from-priority-queue
template<typename T>
class priority_queue_with_remove : public std::priority_queue<T, std::vector<T>, comparator>
{
  public:

      bool remove(const T& value) {
        auto it = std::find(this->c.begin(), this->c.end(), value);
        if (it != this->c.end()) {
            this->c.erase(it);
            std::make_heap(this->c.begin(), this->c.end(), this->comp);
            return true;
       }
       else {
        return false;
       }
 }
};


struct BicriteriaDijkstra {
    RiskMap risk_map;
};

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
