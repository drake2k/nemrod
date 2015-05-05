#include "Geodata.hpp"
#include "Diagnostics.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace Nemrod;

void Nemrod::move_point_in_direction(int direction_deg, int distance_meter, Point& point) {
    std::streamsize prevCoutPrecision = std::cout.precision();
    std::cout.precision(5);
    std::ios::fmtflags prevFlags = std::cout.setf(std::ios::fixed);
    TRACE("move_point_in_direction: " << point.GetLatitude() << "," << point.GetLongitude() << " by " << distance_meter << " @ " << direction_deg)
    float newRadLat, newRadLong;

    const double angularDistance = (double)distance_meter/EARTH_RADIUS;
    
    const double pointRadLat = point.GetLatitude()*M_PI/180;
    const double pointRadLong = point.GetLongitude()*M_PI/180;
    
    newRadLat = asin(sin(pointRadLat) * cos(angularDistance) + 
                     cos(pointRadLat) * sin(angularDistance) * cos(direction_deg*M_PI/180) );
    
    newRadLong = pointRadLong + 
                 atan2(sin(direction_deg*M_PI/180) * sin(angularDistance) *  cos(pointRadLat),
                       cos(angularDistance) - sin(pointRadLat) * sin(newRadLat));

    newRadLong = fmod((newRadLong+3*M_PI), (2*M_PI)) - M_PI; // normalise to -180..+180°
    
    point.SetLatitude(newRadLat*180/M_PI);
    point.SetLongitude(newRadLong*180/M_PI);
    TRACE("move_point_in_direction: resulting point is: " <<  point.GetLatitude() << "," << point.GetLongitude())
    std::cout.precision(prevCoutPrecision);
    std::cout.setf(prevFlags);
}

