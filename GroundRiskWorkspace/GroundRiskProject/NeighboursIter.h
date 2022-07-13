#ifndef NEIGHBOURSITER_H
#define NEIGHBOURSITER_H

#include <vector>

#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

class RiskMap;

class NeighboursIter
{
public:
    NeighboursIter(RiskMap* map, int search_limit, std::vector<int> p);
    ~NeighboursIter();
 
    RiskMap* map;
    int search_limit;
    std::vector<int> p;
    int current_x;
    int current_y;
    int x_from;
    int x_to;
    int y_from;
    int y_to;

public:
    struct Iterator 
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::vector<int>;   //Coord
        using pointer           = std::vector<int>*;  // or also value_type* 
        using reference         = std::vector<int>&;  // or also value_type&
        
        Iterator(pointer ptr) : m_ptr(ptr) {}
        
        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        // Prefix increment
        Iterator& operator++() { m_ptr++; return *this; }  

        // Postfix increment
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };     

        private:

            pointer m_ptr;
    };
    
    Iterator begin() { 
        // TODO: fix
        std::vector<int> temp = {0,0};
        
        return Iterator(&temp);
    }
 
    Iterator end() {    //used to determine when the boundary has been reached: it should never be accessed directly
        // TODO: fix
        std::vector<int> temp = {0,0};
        
        return Iterator(&temp);
    }

};


#endif // NEIGHBOURSITER_H
