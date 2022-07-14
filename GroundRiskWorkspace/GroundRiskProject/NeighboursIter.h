#ifndef NEIGHBOURSITER_H
#define NEIGHBOURSITER_H

#include "risks.h"

#include <vector>
#include <iostream>

#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

class RiskMap;

class NeighboursIter
{
public:
    NeighboursIter(Coord p, Coord current_point, int x_from, int y_from, int x_to, int y_to, int mapOffset);
    ~NeighboursIter();

private: 
    Coord p;

    int x_from;
    int x_to;
    int y_from;
    int y_to;
    
    int mapOffset;
    
    Coord current_point;
    Coord* current_point_ptr;

public:
//https://anderberg.me/2016/07/04/c-custom-iterators/
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Coord;
    using pointer           = Coord*;
    using reference         = Coord&;
    
    reference operator*() const { 
        return *current_point_ptr; 
        }
    pointer operator->() { return current_point_ptr; }

    // Prefix increment
    NeighboursIter& operator++() { next(); return *this; }  
    // Postfix increment
    NeighboursIter operator++(int) { NeighboursIter tmp = *this; next(); return tmp; }

    void next() {
        if (this->current_point.x + this->mapOffset == this->p.x && this->current_point.y + this->mapOffset == this->p.y) {
            
            this->propagate();
        }

        if (this->current_point.y > this->y_to) {
            current_point = Coord(-1, -1);
            return;
        }

        //?????
        //Coord res = Coord(this->current_point.x + this->mapOffset, this->current_point.y + this->mapOffset);

        this->propagate();
        }
        
    void propagate() {
        //std::cout << "propagate: " << current_point.x <<  " " << current_point.y << "\n";
        if (this->current_point.x >= this->x_to) {
            this->current_point.x = this->x_from;
            this->current_point.y += 1;
            //std::cout << "increase y\n";
        }
        else {
            this->current_point.x += 1;
            //std::cout << "increase x\n";
        }
    }

    friend bool operator== (const NeighboursIter& a, const NeighboursIter& b) { return a.current_point == b.current_point; };
    friend bool operator!= (const NeighboursIter& a, const NeighboursIter& b) { return a.current_point != b.current_point; };
};

class Neighbours
{
public:
    Neighbours(RiskMap* map, int search_limit, Coord p);
    ~Neighbours();
    
    NeighboursIter begin();
    NeighboursIter end();
 
    RiskMap* map;
    int search_limit;
    Coord p;
    int x_from;
    int x_to;
    int y_from;
    int y_to;
};

#endif // NEIGHBOURSITER_H
