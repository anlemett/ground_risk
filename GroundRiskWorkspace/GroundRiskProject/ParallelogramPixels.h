#ifndef PARALLELOGRAMPIXELS_H
#define PARALLELOGRAMPIXELS_H

#include "risks.h"

#include <bits/stdc++.h>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

class ParallelogramPixelsIter
{
public:
    ParallelogramPixelsIter(Coord<float> l, Coord<float> r, Coord<float> b, Coord<float> t, Coord<int> current_point);
    ~ParallelogramPixelsIter();
   
    //https://anderberg.me/2016/07/04/c-custom-iterators/
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    
    bool operator== (const ParallelogramPixelsIter& rhs);

    bool operator!= (const ParallelogramPixelsIter& rhs);
    
    Coord<int>& operator*() const;
    
    Coord<int>* operator->();

    // Prefix increment
    ParallelogramPixelsIter& operator++();
    // Postfix increment
    ParallelogramPixelsIter operator++(int);
    
private: 

    void next();
 
    std::pair<int, int> yRange(); //add float x ?
    
    int getMinY(Coord<float> p0, Coord<float> p1, float x); // float x ??
    int getMaxY(Coord<float> p0, Coord<float> p1, float x); // float x ??

    Coord<float> l;
    Coord<float> r;
    Coord<float> b;
    Coord<float> t;

    bool iter_end;
    std::pair<int, int> current_range;    
    Coord<int> current_point;
    Coord<int>* current_point_ptr;
};


class ParallelogramPixels
{
public:
    ParallelogramPixels(Side origin_side, Side destination_side);
    ~ParallelogramPixels();
    
    ParallelogramPixelsIter begin();
    ParallelogramPixelsIter end();
    
//private:
public: //temp
    Side origin_side;
    Side destination_side;
    
    Coord<float> l;
    Coord<float> r;
    Coord<float> b;
    Coord<float> t;
};

#endif // PARALLELOGRAMPIXELS_H
