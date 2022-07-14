#ifndef AIRRISKINSTANCE_H
#define AIRRISKINSTANCE_H

#include "risks.h"

class AirRiskInstance
{
public:
    AirRiskInstance();
    ~AirRiskInstance();
    
    float ComputeAirRisk(Path& path);
    
public: // change to private + set functions or change constructor
    std::vector<std::vector<int>> map;
    int total_time_s;

};

#endif // AIRRISKINSTANCE_H
