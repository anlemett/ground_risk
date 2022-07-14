#ifndef __RISKS_H__
#define __RISKS_H__

#include <vector>
#include <map>

typedef std::map<std::vector<unsigned char>, int> ColorsMapType;

struct Coord {
    int x;
    int y;
    
    Coord() {
    }   
    
    Coord(int x, int y) {
        this->x = x;
        this->y = y;
    }
    
    bool operator==(const Coord& p) const {
        return (x == p.x && y == p.y);
    }
    
    bool operator!=(const Coord& p) const {
        return (x != p.x || y != p.y);
    }

    Coord& operator =(const Coord& p) {
        x = p.x;
        y = p.y;
        return *this;
    }
};

// https://www.techiedelight.com/use-struct-key-std-unordered_map-cpp/
// The specialized hash function for `unordered_map` keys
struct hash_fn
{
    std::size_t operator() (const Coord &p) const
    {
        std::size_t h1 = std::hash<int>()(p.x);
        std::size_t h2 = std::hash<int>()(p.y);
 
        return h1 ^ h2;
    }
};

struct Path {
    std::vector<Coord>  path;
    float linear_combination_weight;
    int risk;
    float length_m;
    float alpha;
    
    Path() {
    }
    
    Path(std::vector<Coord>  path, float linear_combination_weight, int risk, float length_m, float alpha) {
        this->path = path;
        this->linear_combination_weight = linear_combination_weight;
        this->risk = risk;
        this->length_m = length_m;
        this->alpha = alpha;
    }
};


#endif //__RISKS_H__