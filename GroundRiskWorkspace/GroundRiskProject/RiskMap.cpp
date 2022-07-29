#include "RiskMap.h"
#include "Neighbours.h"
#include "ParallelogramPixels.h"

#include <math.h> 

RiskMap::RiskMap()
{
    m_per_pixel = 1000.0/(131.0/2.0);
    offset = 0;
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


float RiskMap::lengthM(Coord<int> p1, Coord<int> p2) {
    double x_diff = (p1.x - p2.x);
    double y_diff = (p1.y - p2.y);
    double length_px_squared = pow(y_diff, 2) + pow(x_diff, 2);
    double length_px = sqrt(length_px_squared);
    return length_px * this->m_per_pixel + 0.0000001;
}


Neighbours RiskMap::neighboursWithin(Coord<int> p, int search_limit) {
    Neighbours neighbours(this, search_limit, p);
    return neighbours;
}


int RiskMap::risk(Coord<int> p1, Coord<int> p2, float r_m) {
    std::pair<Side, Side>  sides = this->parallelogramFromTwoPoints(p1, p2, r_m, this->m_per_pixel);
    int r = this->parallelogramRisk(sides.first, sides.second);
    return r;
}
   

std::pair<Side, Side> RiskMap::parallelogramFromTwoPoints(Coord<int> p1, Coord<int> p2, float r_m, float m_per_pixel) {
    
    double y_diff = -(p2.y-p1.y);
    double x_diff = (p2.x-p1.x);
    
    double slope = atan2(y_diff, x_diff);
    int rect_width = ceil((r_m /m_per_pixel));
    
    Coord<double> orig_p1 = {
        p1.x + (double)sin(slope) * rect_width,
        p1.y + (double)cos(slope) * rect_width
    };
    
    Coord<double> orig_p2 = {
        p1.x + (double)sin(slope + M_PI) * rect_width,
        p1.y + (double)cos(slope + M_PI) * rect_width
    };

    Coord<double> dest_p1 = {
        p2.x + (double)sin(slope) * rect_width,
        p2.y + (double)cos(slope) * rect_width
    };
    
    Coord<double> dest_p2 = {
        p2.x + (double)sin(slope + M_PI) * rect_width,
        p2.y + (double)cos(slope + M_PI) * rect_width
    };

    //std::cout << "parallelogramFromTwoPoints: " << orig_p1.x << " " << orig_p1.y << ":" << orig_p2.x << " " << orig_p2.y;
    //std::cout << ":" << dest_p1.x << " " << dest_p1.y << ":" << dest_p2.x << " " << dest_p2.y << "\n";
    return std::make_pair(std::make_pair(orig_p1, orig_p2), std::make_pair(dest_p1, dest_p2));
}


int RiskMap::parallelogramRisk(Side origin_side, Side destination_side) {
    int pop = 0;
    
    ParallelogramPixels* rect = new ParallelogramPixels(origin_side, destination_side);
    
    for (auto coord: *rect) {
        if (0 <= coord.x && coord.x < width() && 0 <= coord.y && coord.y < height()) {
            //std::cout << "pixel in rect: " << coord.x << " " << coord.y << "\n";
            pop = pop + riskAt(coord);
        }
    }
    
    delete rect;
    
    //std::cout << "parallelogramRisk: "<< pop<< "\n";
    return pop;
}

int RiskMap::riskAt(Coord<int> coord) {
    return map.at(coord.y).at(coord.x);
}


