#include "Geodata.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace Nemrod;

void move_point_in_direction(int direction_deg, int distance_meter, Point& point) {
    float newLat, newLong;
    
    newLat = asin(sin(point.GetLatitude()) * cos(distance_meter/EARTH_RADIUS) + 
             cos(point.GetLatitude()) * sin(distance_meter/EARTH_RADIUS) * cos(direction_deg*M_PI/180) );
    
    newLong = point.GetLongitude() + 
              atan2(sin(direction_deg*M_PI/180) * sin(distance_meter/EARTH_RADIUS) *  cos(point.GetLatitude()), 
                         cos(distance_meter/EARTH_RADIUS) - sin(point.GetLatitude()) * sin(newLat));

    point.SetLatitude(newLat);
    point.SetLongitude(newLong);
}

