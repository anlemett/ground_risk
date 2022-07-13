#include "RiskMap.h"

RiskMap::RiskMap()
{
}

RiskMap::~RiskMap()
{
}

int RiskMap::height() {
    int h = this->map.size() - this->offset*2;
    if (h<1) {
        std::cerr << "The map is smaller than the offset" << std::endl;
        exit(1);
    }
    return h;
}

int RiskMap::width() {
    int w = this->map.at(0).size() - this->offset*2;
    if (w<1) {
        std::cerr << "The map is smaller than the offset" << std::endl;
        exit(1);
    }
    return w;
}

NeighboursIter RiskMap::neighboursWithin(std::vector<int> p, int search_limit) {
    NeighboursIter* iter = new NeighboursIter(this, search_limit, p);
    return *iter;
}
