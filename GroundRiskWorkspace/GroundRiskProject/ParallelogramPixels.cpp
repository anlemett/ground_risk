#include "ParallelogramPixels.h"
#include <math.h>

bool compareCoordXFirst(Coord<float> p1, Coord<float> p2)
{
    if (p1.x == p2.x) {
        return p1.y < p2.y;
    }
    else {
        return p1.x < p2.x;
    }
}

bool compareCoordYFirst(Coord<float> p1, Coord<float> p2)
{
    if (p1.y == p2.y) {
        return p1.x < p2.x;
    }
    else {
        return p1.y < p2.y;
    }
}


// ParallelogramPixelsIter
ParallelogramPixelsIter::ParallelogramPixelsIter(Coord<float> l, Coord<float> r, Coord<float> b, Coord<float> t,
                                                 Coord<int> current_point) {

    this->l = l;
    this->r = r;
    this->b = b;
    this->t = t;
    
    //this->iter_end = iter_end;
    
    this->current_point = current_point;
    
    if (current_point.x == round(l.x)-1) {
        current_point.x = floor(l.x);
        current_point.y = 0;

        current_range = yRange();
        current_point.y = this->current_range.first;
//        std::cout << "current_range: " << current_range.first << " "<< current_range.second << "\n";
    }
    
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
//    std::cout << "inside next()";
//    std::cout << "current_point was " << current_point << "\n";
           
    if (current_point.x <= r.x ) {
        if (current_point.y > 5000) {
            std::cout << "y is > 5000";
        }
        if (current_point.y + 1 > current_range.second) {
//            std::cout << "increase x\n";
            current_point.x ++;
            current_range = yRange();
            current_point.y = current_range.first;
        }
        else {
//            std::cout << "increase y\n";
            current_point.y ++;
        }
    }
    else {
        current_point.x = round(r.x) + 1;
        current_point.y = round(r.y);
    }
    //std::cout << "current_point is " << current_point << "\n";
}


std::pair<int, int> ParallelogramPixelsIter::yRange() {

    if ((l.x != b.x) && (abs(l.x - b.x) < 0.001) || (t.y != r.y) && (abs(t.y - r.y)) < 0.001) {
        std::cerr << "Numerical errors detected!" << std::endl;
        exit(1);
    }

    int y1 = 0;
    int y2 = 0;
    

    if (t.x < b.x) {
//        std::cout << "1\n";
        //   -------      ----
        //  /     /   or   \  \
        // ------           \  \
        //                   \  \
        //                    ----

        if (current_point.x < t.x) {
//            std::cout << "11\n";
            y1 = getMinY(l, b, current_point.x);
            y2 = getMaxY(l, t, current_point.x);
        } 
        else if (current_point.x >= t.x && current_point.x <= b.x) {
//            std::cout << "12\n";
            y1 = getMinY(l, b, current_point.x);
            y2 = getMaxY(t, r, current_point.x);
        }
        else {
//            std::cout << "13\n";
            y1 = getMinY(b, r, current_point.x);
            y2 = getMaxY(t, r, current_point.x);
        }
    }
    else if (t.x > b.x && t.y != b.y) {
//        std::cout << "2\n";
        //          -------           ----
        //           \     \   or    /   /
        //            ------        /   /
        //                         /   /
        //                         ----

        if (current_point.x < b.x) {
//            std::cout << "21\n";
            y1 = getMinY(l, b, current_point.x);
            y2 = getMaxY(l, t, current_point.x);
            }
        else if (current_point.x >= b.x && current_point.x <= t.x) {
//            std::cout << "22\n";
            y1 = getMinY(b, r, current_point.x);
            y2 = getMaxY(l, t, current_point.x);
        }
        else {
//            std::cout << "23\n";
            y1 = getMinY(b, r, current_point.x);
            y2 = getMaxY(t, r, current_point.x);
        }
    }
    else if (t.x == b.x) {
//        std::cout << "3\n";
        //          /\
        //          \/
        if (current_point.x < t.x) {
//            std::cout << "31\n";
            y1 = getMinY(l, b, current_point.x);
            y2 = getMaxY(l, t, current_point.x);
        }
        else {
//            std::cout << "32\n";
            y1 = getMinY(b, r, current_point.x);
            y2 = getMaxY(t, r, current_point.x);
        }
    }
    else if (t.x > b.x && b.y == t.y) {
//        std::cout << "4\n";
        y1 = getMinY(l, t, current_point.x);
        y2 = getMaxY(b, r, current_point.x);
    }
    else {
        std::cerr << "An unknown case occured!" << std::endl;
        exit(1);
    }

//    std::cout << "y1: " << y1 << " y2: " << y2 << "\n";
    return std::make_pair(y1, y2);
}


int ParallelogramPixelsIter::getMinY(Coord<float> p0, Coord<float> p1, float x) { 
    // Thanks to Markus Jarderot from https://stackoverflow.com/questions/5610616/finding-all-pixels-at-least-partially-within-an-arbitrarily-oriented-rectangle
    // for inspiration.

    float x0 = p0.x;
    float y0 = p0.y;
    float x1 = p1.x;
    float y1 = p1.y;
    
//    std::cout << "y0: "<< y0 << " y1: " << y1 << "\n";

    if (abs(x0 - x1) < 0.00001) {
        return (int)floor(y0);
    }

    float slope = (y1 - y0)/(x1 - x0);

    if (slope >= 0.0) {
//        std::cout << "slope>=0\n";
        float xl = std::max((float)x0, (float)(x - 0.5));
        float res = y0 + slope * (xl - x0) + 0.00001;
        return round(res);
    }
    else {
//        std::cout << "slope<0\n";
        float xr = std::min((float)x1, (float)(x + 0.5));
//        std::cout << "slope: " << slope << "\n";
        float temp = slope * (xr - x0);
//        std::cout << "temp: " << temp << "\n";
        float res = y0 + slope * (xr - x0) + 0.00001;
//        std::cout << "res: " << res << " round(res): " << round(res) << "\n";
        return round(res);
    }
}


int ParallelogramPixelsIter::getMaxY(Coord<float> p0, Coord<float> p1, float x) { 
    // Thanks to Markus Jarderot from https://stackoverflow.com/questions/5610616/finding-all-pixels-at-least-partially-within-an-arbitrarily-oriented-rectangle
    // for inspiration.

    float x0 = p0.x;
    float y0 = p0.y;
    float x1 = p1.x;
    float y1 = p1.y;

    if (abs(x0 - x1) < 0.00001) {
        return std::ceil(y1);
    }

    float slope = (y1 - y0)/(x1 - x0);

    if (slope >= 0.0) {
//        std::cout << "max slope>=0\n";
        float xr = std::min((float)x1, (float)(x + 0.5));
        float temp = slope* (xr - x0);
//        std::cout << "temp: " << temp << "\n";
        float res = y0 + slope * (xr - x0) - 0.00001;
//        std::cout << "res: " << res << " round(res): " << round(res) << "\n";

        return std::round(res);
    }
    else {
//        std::cout << "max slope<0\n";
        float xl = std::max((float)x0, (float)(x - 0.5));
        float res = y0 + slope * (xl - x0) - 0.00001;
        return std::round(res);
    }
}


//ParallelogramPixels
ParallelogramPixels::ParallelogramPixels(Side origin_side, Side destination_side)
{
    // TODO: add round_if_needed();
    this->origin_side = origin_side;
    this->destination_side = destination_side;
    
    std::vector<Coord<float>> points = {origin_side.first, origin_side.second, destination_side.first, destination_side.second};
    
    // constructor method, Deep copy 
    std::vector<Coord<float>> l_mid1_mid2_r(points);
    
    sort(l_mid1_mid2_r.begin(), l_mid1_mid2_r.end(), compareCoordXFirst);
    
    l = l_mid1_mid2_r.at(0);
    Coord<float> mid1 = l_mid1_mid2_r.at(1), mid2 = l_mid1_mid2_r.at(2);
    r = l_mid1_mid2_r.at(3);
        
    std::vector<Coord<float>> mid_points = {mid1, mid2};
    
    std::vector<Coord<float>> b_t(mid_points);
    
    std::sort(b_t.begin(), b_t.end(), compareCoordYFirst);
    
    b = b_t.at(0);
    t = b_t.at(1);
    
    std::cout <<"inside constructor: "<< l << " " << t << " " << b<< " "<<r << "\n";
}


ParallelogramPixels::~ParallelogramPixels()
{
}


ParallelogramPixelsIter ParallelogramPixels::begin() { 
    Coord<int> current_point = {round(l.x)-1, round(l.y)};
    std::cout << "inside begin(): l=" << l << " r=" << r << " b=" << b << " t=" << t << "current_point"<< current_point<< "\n";
    return ParallelogramPixelsIter(l, r, b, t, current_point);
}
 
 
ParallelogramPixelsIter ParallelogramPixels::end() {//used to determine when the boundary has been reached: it should not be accessed directly
    Coord<int> current_point = {round(r.x)+1, round(r.y)};
    std::cout << "inside end(): l=" << l << " r=" << r << " b=" << b << " t=" << t << "current_point"<< current_point<< "\n";
    return ParallelogramPixelsIter(l, r, b, t, current_point);
}

