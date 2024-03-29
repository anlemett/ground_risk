#ifndef NEIGHBOURS_H
#define NEIGHBOURS_H

#include "risks.h"

#include <vector>
#include <iostream>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

class RiskMap;

class NeighboursIter
{
public:
    NeighboursIter(Coord<int> p, Coord<int> current_point, int x_from, int y_from, int x_to, int y_to, int mapOffset);
    ~NeighboursIter();

    //https://anderberg.me/2016/07/04/c-custom-iterators/
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
        
    bool operator== (const NeighboursIter& rhs);
    bool operator!= (const NeighboursIter& rhs);
    
    Coord<int>& operator*() const;
    Coord<int>* operator->();

    // Prefix increment
    NeighboursIter& operator++();
    // Postfix increment
    NeighboursIter operator++(int);

private: 

    void next();
        
    void propagate();

    Coord<int> p;

    int x_from;
    int x_to;
    int y_from;
    int y_to;
    
    int mapOffset;
    
    Coord<int> current_point;
    Coord<int>* current_point_ptr;
};

class Neighbours
{
public:
    Neighbours(RiskMap* map, int search_limit, Coord<int> p);
    ~Neighbours();
    
    NeighboursIter begin();
    NeighboursIter end();
 
    RiskMap* map;
    int search_limit;
    Coord<int> p;
    int x_from;
    int x_to;
    int y_from;
    int y_to;
};

#endif // NEIGHBOURS_H
