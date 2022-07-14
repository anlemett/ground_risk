#include "NeighboursIter.h"
#include "RiskMap.h"

// Neighbours
Neighbours::Neighbours(RiskMap* map, int search_limit, Coord p)
{
    this->map = map;
    this->search_limit = search_limit;
    this->p = p;
    
    int width = map->width();
    int height = map->height();
    
    this->x_from = std::max(0, p.x - map->offset - search_limit);
    this->x_to = std::min(width - 1, p.x - map->offset + search_limit);
    this->y_from = std::max(0, p.y - map->offset - search_limit);
    this->y_to = std::min(height - 1, p.y - map->offset + search_limit);
}

Neighbours::~Neighbours()
{
}

NeighboursIter Neighbours::begin() { 
    
    Coord current_point = Coord(this->x_from, this->y_from);
    return NeighboursIter(this->p, current_point, this->x_from, this->y_from, this->x_to, this->y_to, this->map->offset);
    }
 
 
NeighboursIter Neighbours::end() {//used to determine when the boundary has been reached: it should not be accessed directly

    Coord current_point = Coord(-1, -1);
    return NeighboursIter(this->p, current_point, this->x_from, this->y_from, this->x_to, this->y_to, this->map->offset);   
    }


// NeighboursIter
NeighboursIter::NeighboursIter(Coord p, Coord current_point, int x_from, int y_from, int x_to, int y_to, int mapOffset)
{
    this->p = p;
    this->mapOffset = mapOffset;
   
    this->x_from = x_from;
    this->x_to = x_to;
    this->y_from = y_from;
    this->y_to = y_to;
    
    this->current_point = current_point;
    this->current_point_ptr = &(this->current_point);
}

NeighboursIter::~NeighboursIter()
{
}

