#include "Neighbours.h"
#include "RiskMap.h"

// NeighboursIter
NeighboursIter::NeighboursIter(Coord<int> p, Coord<int> current_point, int x_from, int y_from, int x_to, int y_to, int mapOffset)
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


bool NeighboursIter::operator== (const NeighboursIter& rhs) {
    return current_point == rhs.current_point;
};

bool NeighboursIter::operator!= (const NeighboursIter& rhs) {
    return !(*this == rhs);
};
    
Coord<int>& NeighboursIter::operator*() const { 
    return *current_point_ptr; 
}

Coord<int>* NeighboursIter::operator->() {
    return current_point_ptr;
}

// Prefix increment
NeighboursIter& NeighboursIter::operator++() {
    next();
    return *this;
}  

// Postfix increment
NeighboursIter NeighboursIter::operator++(int) {
    NeighboursIter tmp = *this;
    next();
    return tmp;
}

void NeighboursIter::next() {
    if (this->current_point.x + this->mapOffset == this->p.x && this->current_point.y + this->mapOffset == this->p.y) {
            
        this->propagate();
    }

    if (this->current_point.y > this->y_to) {
        std::cout << "!!this->current_point.y > this->y_to!!" << this->current_point.y << " " << this->y_to << "\n";
        current_point = Coord<int>(this->x_to, this->y_to);
        return;
    }

    //?????
    //Coord res = Coord(this->current_point.x + this->mapOffset, this->current_point.y + this->mapOffset);

    this->propagate();
    }
        

void NeighboursIter::propagate() {
    //std::cout << "propagate: " << current_point.x <<  " " << current_point.y << "\n";
    if (this->current_point.x >= this->x_to) {
        this->current_point.x = this->x_from;
        this->current_point.y += 1;
        //std::cout << "increase y" << this->current_point.y <<"\n";
    }
    else {
        this->current_point.x += 1;
        //std::cout << "increase x" << this->current_point.x <<"\n";
    }
}


// Neighbours
Neighbours::Neighbours(RiskMap* map, int search_limit, Coord<int> p)
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
    Coord<int> current_point = Coord<int>(this->x_from, this->y_from);
    return NeighboursIter(this->p, current_point, this->x_from, this->y_from, this->x_to, this->y_to, this->map->offset);
}
 
 
NeighboursIter Neighbours::end() {//used to determine when the boundary has been reached: it should not be accessed directly
    Coord<int> current_point = Coord<int>(this->x_to, this->y_to);
    return NeighboursIter(this->p, current_point, this->x_from, this->y_from, this->x_to, this->y_to, this->map->offset);   
}
