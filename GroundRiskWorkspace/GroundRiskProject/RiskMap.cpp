#include "RiskMap.h"
#include "NeighboursIter.h"

#include <math.h> 

RiskMap::RiskMap()
{
}

RiskMap::~RiskMap()
{
}

int RiskMap::height() {
    int h = this->map.size() - this->offset*2;
    if (h<1) {
        std::cerr << "The map is smaller than the offset" << std::endl;
        exit(1);
    }
    return h;
}

int RiskMap::width() {
    int w = this->map.at(0).size() - this->offset*2;
    if (w<1) {
        std::cerr << "The map is smaller than the offset" << std::endl;
        exit(1);
    }
    return w;
}

std::vector<Coord> RiskMap::parallelogramFromTwoPoints(Coord p1, Coord p2, float r_m, float m_per_pixel) {
    
    float y_diff = -(p2.y-p1.y);
    float x_diff = (p2.x-p1.x);
    
    Coord point1_temp = {0,0}, point2_temp = {0,0}; //remove
    std::vector<Coord> temp = {point1_temp, point2_temp}; //remove
    return temp; //remove
}
//        let y_diff = -(p2.y-p1.y) as f64;
//        let x_diff = (p2.x-p1.x) as f64;
//        let slope = y_diff.atan2(x_diff);
//        let rect_width = (r_m /m_per_pixel).ceil();

//        let orig_p1 = Coord{
//            x: p1.x as f64 + slope.sin() * rect_width,
//            y: p1.y as f64 + slope.cos() * rect_width
//        };

//        let orig_p2 = Coord{
//            x: p1.x as f64 + (slope + std::f64::consts::PI).sin() * rect_width,
//            y: p1.y as f64 + (slope + std::f64::consts::PI).cos() * rect_width
//        };

//        let dest_p1 = Coord{
//            x: p2.x as f64 + slope.sin() * rect_width,
//            y: p2.y as f64 + slope.cos() * rect_width
//        };

//        let dest_p2 = Coord{
//            x: p2.x as f64 + (slope + std::f64::consts::PI).sin() * rect_width,
//            y: p2.y as f64 + (slope + std::f64::consts::PI).cos() * rect_width
//        };

//        return ((orig_p1, orig_p2), (dest_p1, dest_p2))
//    }


int RiskMap::risk(Coord p1, Coord p2, float r_m) {
    std::vector<Coord> sides = this->parallelogramFromTwoPoints(p1, p2, r_m, this->m_per_pixel);
    Coord orig_side = sides.at(0);
    Coord dest_side = sides.at(1);
    
    //int r = this->parallelogramRisk(orig_side, dest_side);
    int r = 0; //remove
    return r;
}
   
float RiskMap::lengthM(Coord p1, Coord p2) {
    float x_diff = (p1.x - p2.x);
    float y_diff = (p1.y - p2.y);
    float length_px_squared = pow(y_diff, 2) + pow(x_diff, 2);
    float length_px = sqrt(length_px_squared);
    return length_px * this->m_per_pixel + 0.0000001;
}

Neighbours RiskMap::neighboursWithin(Coord p, int search_limit) {
    Neighbours* neighbours = new Neighbours(this, search_limit, p);
    return *neighbours;
}
