#include "ParallelogramPixels.h"

#include <math.h>

bool compareCoordXFirst(Coord<double> p1, Coord<double> p2)
{
    if (p1.x == p2.x) {
        return p1.y < p2.y;
    }
    else {
        return p1.x < p2.x;
    }
}

bool compareCoordYFirst(Coord<double> p1, Coord<double> p2)
{
    if (p1.y == p2.y) {
        return p1.x < p2.x;
    }
    else {
        return p1.y < p2.y;
    }
}


// ParallelogramPixelsIter
ParallelogramPixelsIter::ParallelogramPixelsIter(Coord<double> l, Coord<double> r, Coord<double> b, Coord<double> t,
                                                 Coord<int> current_point) {

    this->l = l;
    this->r = r;
    this->b = b;
    this->t = t;
    
    this->current_point = current_point;
    
    this->current_range = yRange();
    this->current_point.y = this->current_range.first;
    
    this->current_point_ptr = &(this->current_point);
}


ParallelogramPixelsIter::~ParallelogramPixelsIter()
{
}


bool ParallelogramPixelsIter::operator==(const ParallelogramPixelsIter& rhs)
{
    return current_point == rhs.current_point;
}


bool ParallelogramPixelsIter::operator!=(const ParallelogramPixelsIter& rhs)
{
    return !(*this == rhs);
}

Coord<int>& ParallelogramPixelsIter::operator*() const { 
    return *current_point_ptr; 
}

Coord<int>* ParallelogramPixelsIter::operator->() {
    return current_point_ptr;
}


ParallelogramPixelsIter& ParallelogramPixelsIter::operator++() {
    this->next();
    return *this;
}

ParallelogramPixelsIter ParallelogramPixelsIter::operator++(int) {
    ParallelogramPixelsIter tmp = *this;
    this->next();
    return tmp; 
}

void ParallelogramPixelsIter::next() {
           
    if (current_point.x <= r.x ) {
        if (current_point.y > 5000) {
            std::cout << "y is > 5000";
        }
        if (current_point.y + 1 > current_range.second) {
            current_point.x ++;
            current_range = yRange();
            current_point.y = current_range.first;
        }
        else {
            current_point.y ++;
        }
    }
    else {
        current_point.x = floor(r.x) + 1;
        current_point.y = floor(r.y) + 1;
    }

}


std::pair<int, int> ParallelogramPixelsIter::yRange() {

    int y1 = 0;
    int y2 = 0;
    

    if (t.x < b.x) {
        /*   -------      ----
        //  /     /   or   \  \
        // ------           \  \
        //                   \  \
        //                    ---- */

        if (current_point.x < t.x) {
            y1 = getMinY(l, b, current_point.x);
            y2 = getMaxY(l, t, current_point.x);
        } 
        else if (current_point.x >= t.x && current_point.x <= b.x) {
            y1 = getMinY(l, b, current_point.x);
            y2 = getMaxY(t, r, current_point.x);
        }
        else {
            y1 = getMinY(b, r, current_point.x);
            y2 = getMaxY(t, r, current_point.x);
        }
    }
    else if (t.x > b.x && t.y != b.y) {
        /*          -------           ----
        //           \     \   or    /   /
        //            ------        /   /
        //                         /   /
        //                         ----   */

        if (current_point.x < b.x) {
            y1 = getMinY(l, b, current_point.x);
            y2 = getMaxY(l, t, current_point.x);
            }
        else if (current_point.x >= b.x && current_point.x <= t.x) {
            y1 = getMinY(b, r, current_point.x);
            y2 = getMaxY(l, t, current_point.x);
        }
        else {
            y1 = getMinY(b, r, current_point.x);
            y2 = getMaxY(t, r, current_point.x);
        }
    }
    else if (t.x == b.x) {
        /*          /\
        //          \/  */
        if (current_point.x < t.x) {
            y1 = getMinY(l, b, current_point.x);
            y2 = getMaxY(l, t, current_point.x);
        }
        else {
            y1 = getMinY(b, r, current_point.x);
            y2 = getMaxY(t, r, current_point.x);
        }
    }
    else if (t.x > b.x && b.y == t.y) {
        y1 = getMinY(l, t, current_point.x);
        y2 = getMaxY(b, r, current_point.x);
    }
    else {
        std::cerr << "An unknown case occured!" << std::endl;
        exit(1);
    }
    //std::cout << "YRange: " << y1 << " " << y2 << "\n";
    return std::make_pair(y1, y2);
}


int ParallelogramPixelsIter::getMinY(Coord<double> p0, Coord<double> p1, double x) { 
    // Thanks to Markus Jarderot from https://stackoverflow.com/questions/5610616/finding-all-pixels-at-least-partially-within-an-arbitrarily-oriented-rectangle
    // for inspiration.

    double x0 = p0.x;
    double y0 = p0.y;
    double x1 = p1.x;
    double y1 = p1.y;
    
    if (abs(x0 - x1) < 0.00001) {
        return (int)floor(y0);
    }

    double slope = (y1 - y0)/(x1 - x0);

    if (slope >= 0.0) {
        double xl = std::max((double)x0, (double)(x - 0.5));
        double res = y0 + slope * (xl - x0) + 0.00001;
        return round(res);
    }
    else {
        double xr = std::min((double)x1, (double)(x + 0.5));
        double res = y0 + slope * (xr - x0) + 0.00001;
        return round(res);
    }
}


int ParallelogramPixelsIter::getMaxY(Coord<double> p0, Coord<double> p1, double x) { 
    // Thanks to Markus Jarderot from https://stackoverflow.com/questions/5610616/finding-all-pixels-at-least-partially-within-an-arbitrarily-oriented-rectangle
    // for inspiration.

    double x0 = p0.x;
    double y0 = p0.y;
    double x1 = p1.x;
    double y1 = p1.y;
    
    if (abs(x0 - x1) < 0.00001) {
        return std::ceil(y1);
    }

    double slope = (y1 - y0)/(x1 - x0);

    if (slope >= 0.0) {
        double xr = std::min((double)x1, (double)(x + 0.5));
        double res = y0 + slope * (xr - x0) - 0.00001;
        return std::round(res);
    }
    else {
        double xl = std::max((double)x0, (double)(x - 0.5));
        double res = y0 + slope * (xl - x0) - 0.00001;
        return std::round(res);
    }
}


//ParallelogramPixels
ParallelogramPixels::ParallelogramPixels(Side origin_side, Side destination_side)
{
    this->origin_side = origin_side;
    this->destination_side = destination_side;

    this->origin_side.first.round_if_needed();
    this->origin_side.second.round_if_needed();
    this->destination_side.first.round_if_needed();
    this->destination_side.second.round_if_needed();
    
    std::vector<Coord<double>> points = {origin_side.first, origin_side.second, destination_side.first, destination_side.second};
    
    // constructor method, Deep copy 
    std::vector<Coord<double>> l_mid1_mid2_r(points);
    
    sort(l_mid1_mid2_r.begin(), l_mid1_mid2_r.end(), compareCoordXFirst);
    
    l = l_mid1_mid2_r.at(0);
    Coord<double> mid1 = l_mid1_mid2_r.at(1), mid2 = l_mid1_mid2_r.at(2);
    r = l_mid1_mid2_r.at(3);
        
    std::vector<Coord<double>> mid_points = {mid1, mid2};
    
    std::vector<Coord<double>> b_t(mid_points);
    
    std::sort(b_t.begin(), b_t.end(), compareCoordYFirst);
    
    b = b_t.at(0);
    t = b_t.at(1);
}


ParallelogramPixels::~ParallelogramPixels()
{
}


ParallelogramPixelsIter ParallelogramPixels::begin() { 
    Coord<int> current_point = {(int)floor(l.x), 0};
    return ParallelogramPixelsIter(l, r, b, t, current_point);
}
 
 
ParallelogramPixelsIter ParallelogramPixels::end() {//used to determine when the boundary has been reached: it should not be accessed directly
    Coord<int> current_point = {(int)floor(r.x)+1, (int)floor(r.y)+1};
    return ParallelogramPixelsIter(l, r, b, t, current_point);
}
