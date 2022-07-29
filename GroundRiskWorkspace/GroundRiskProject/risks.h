#ifndef __RISKS_H__
#define __RISKS_H__

#include <vector>
#include <map>
#include <iostream>
#include <math.h>

#include <bits/stdc++.h>
#include <unordered_map>
#include <queue>

typedef std::map<std::vector<unsigned char>, int> ColorsMapType;

template <typename T> 
struct Coord {
    T x;
    T y;
    
    Coord() {
    }   
    
    Coord(T x, T y) {
        this->x = x;
        this->y = y;
    }
    
    friend std::ostream& operator << (std::ostream& os, const Coord<T>& coord) {
        os << "("<< coord.x << ", " << coord.y << ") ";
        return os;
    }

    bool operator==(const Coord<int>& p) const {
        return (x == p.x && y == p.y);
    }
    
    bool operator!=(const Coord<int>& p) const {
        return (x != p.x || y != p.y);
    }

    bool operator==(const Coord<double>& p) const {
        return ((double)x == p.x && (double)y == p.y);
    }
    
    bool operator!=(const Coord<double>& p) const {
        return ((double)x != p.x || (double)y != p.y);
    }

    Coord& operator =(const Coord<T>& p) {
        x = p.x;
        y = p.y;
        return *this;
    }
    
    void round_if_needed() {
        // A dirty hack to avoid numerical problems
        //FIXME: do something more reliable
        if (abs(round(x) - x) < 0.0001) {
            x = round(x);
        }

        if (abs(round(y) - y) < 0.0001) {
            y = round(y);
        }
    }
};

typedef std::pair<Coord<double>, Coord<double>> Side;

// https://www.techiedelight.com/use-struct-key-std-unordered_map-cpp/
// The specialized hash function for `unordered_map` keys
struct hash_fn
{
    std::size_t operator() (const Coord<int> &p) const
   {
        std::size_t h1 = std::hash<int>()(p.x);
        std::size_t h2 = std::hash<int>()(p.y);
 
        return h1 ^ h2;
    }
};

struct Path {
    std::vector<Coord<int>> path;
    double linear_combination_weight;
    int risk;
    double length_m;
    double alpha;
    
    Path() {
    }
    
    Path(const std::vector<Coord<int>>&  path, double linear_combination_weight, int risk, double length_m, double alpha) {
        this->path = path;
        this->linear_combination_weight = linear_combination_weight;
        this->risk = risk;
        this->length_m = length_m;
        this->alpha = alpha;
    }
    
    friend std::ostream& operator << (std::ostream& os, const Path& path) {
        os << "\nPath: ";
        for (auto element: path.path)
            std::cout << element << " ";
        os << std::endl;
        os << "Ground risk: " << path.risk << std::endl;
        os << "Length: " << path.length_m << std::endl;
        os << "Alpha: " << path.alpha << std::endl;

        return os;
    }
};

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

#endif //__RISKS_H__
