#include "AirRiskInstance.h"
#include "Bresenham.h"
#include <iostream>

AirRiskInstance::AirRiskInstance()
{
}

AirRiskInstance::~AirRiskInstance()
{
}

 float AirRiskInstance::ComputeAirRisk(Path& path)
{
    float air_risk = 0.0;
    int length_px = 0;
    
    for (int i = 0; i < path.path.size()-1; i++) {
        Coord s = path.path.at(i);
        Coord e = path.path.at(i+1); 

        std::vector<Coord> points;
        
        Bresenham* br = new Bresenham(s.x, s.y, e.x, e.y);
        points = br->getPoints();
        delete br;
        
        for(int i=0; i < points.size(); i++){
            int x=points.at(i).x;
            int y=points.at(i).y;
            air_risk += (float)this->map.at(x).at(y);
            length_px += 1;
        }
        
        for (const auto& point : points) {
            air_risk += (float)this->map.at(point.x).at(point.y);
            length_px += 1;
        }
    }
    return (air_risk/(float)length_px)/((float)this->total_time_s);
}
 