#ifndef PARALLELOGRAMPIXELS_H
#define PARALLELOGRAMPIXELS_H

#include "risks.h"
#include <bits/stdc++.h>

#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

class ParallelogramPixelsIter
{
public:
    ParallelogramPixelsIter(Coord l, Coord r, Coord b, Coord t, Coord current_point);
    ~ParallelogramPixelsIter();
   
    //https://anderberg.me/2016/07/04/c-custom-iterators/
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    
    bool operator== (const ParallelogramPixelsIter& rhs);

    bool operator!= (const ParallelogramPixelsIter& rhs);
    
    Coord& operator*() const;
    
    Coord* operator->();

    // Prefix increment
    ParallelogramPixelsIter& operator++();
    // Postfix increment
    ParallelogramPixelsIter operator++(int);
    
private: 

    void next();
 
    std::pair<int, int> yRange(); //add float x ?
    
    int getMinY(Coord p0, Coord p1, float x); // float x ??
    int getMaxY(Coord p0, Coord p1, float x); // float x ??

    Coord l; //<float>
    Coord r; //<float>
    Coord b; //<float>
    Coord t; //<float>

    std::pair<int, int> current_range;    
    Coord current_point;
    Coord* current_point_ptr;
};


class ParallelogramPixels
{
public:
    ParallelogramPixels(Side origin_side, Side destination_side);
    ~ParallelogramPixels();
    
    ParallelogramPixelsIter begin();
    ParallelogramPixelsIter end();
    
private:
    Side origin_side;
    Side destination_side;
    
    Coord l; //<float>
    Coord r; //<float>
    Coord b; //<float>
    Coord t; //<float>
};

#endif // PARALLELOGRAMPIXELS_H
