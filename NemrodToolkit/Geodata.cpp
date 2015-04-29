#include "Geodata.hpp"
#include "Diagnostics.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace Nemrod;

void Nemrod::move_point_in_direction(int direction_deg, int distance_meter, Point& point) {
    TRACE("move_point_in_direction: " << point.GetLatitude() << "," << point.GetLongitude() << " by " << distance_meter << " @ " << direction_deg)
    float newRadLat, newRadLong;

    const float pointRadLat = point.GetLatitude()*M_PI/180;
    const float pointRadLong = point.GetLongitude()*M_PI/180;
    
    newRadLat = asin(sin(pointRadLat) * cos((double)distance_meter/EARTH_RADIUS) + 
             cos(pointRadLat) * sin((double)distance_meter/EARTH_RADIUS) * cos(direction_deg*M_PI/180) );
    
    newRadLong = pointRadLong + 
                 atan2(sin(direction_deg*M_PI/180) * sin((double)distance_meter/EARTH_RADIUS) *  cos(pointRadLat), 
                         cos((double)distance_meter/EARTH_RADIUS) - sin(pointRadLat) * sin(newRadLat));

    point.SetLatitude(newRadLat*180/M_PI);
    point.SetLongitude(newRadLong*180/M_PI);
    TRACE("move_point_in_direction: resulting point is: " <<  point.GetLatitude() << "," << point.GetLongitude())
}

