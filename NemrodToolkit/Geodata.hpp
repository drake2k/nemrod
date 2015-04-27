#ifndef GEODATA_HPP
#define	GEODATA_HPP

namespace Nemrod{
    
    class Point {
    public:
        Point(float lat, float longi) {
            this->SetLatitude(lat);
            this->SetLongitude(longi);
        }

        operator bool() const {
            return _latitude != 91 && _longitude != -91;
        }
        
        bool operator==(const Point& p) const{
            return p._latitude == this->_latitude && p._longitude == this->_longitude;
        }
        
        bool operator!=(const Point& p) const{
            return !(p==*this);
        }

        void SetLongitude(float _longitude) {
            if (_longitude >= -90 && _longitude <= 90)
                this->_longitude = _longitude;
        }

        float GetLongitude() const {
            return _longitude;
        }

        void SetLatitude(float _latitude);

        float GetLatitude() const {
            return _latitude;
        }

    private:
        /*N=90 S=-90*/
        float _latitude = -91;
        /*W=-90 E=90*/
        float _longitude = -91;

    };
    
    /** In meters */
    const int EARTH_RADIUS = 6371000;
    
    /** Cardinal directions, the value is the angle in degrees */
    enum CARDINAL_DIRECTION {
        /** 0 degree */
        NORTH=0,
        /** 45 degrees */
        NORTH_EAST = 45,
        /** 45 degrees */
        NORTH_WEST = -45,
        /** 180 degrees */
        SOUTH=180,
        /** 135 degrees */
        SOUTH_EAST=135,
        /** 225 degrees */
        SOUTH_WEST=225,
        /** 90 degrees */
        EAST=90,
        /** -90 degrees */
        WEST=-90
    };
    
    /**
     * Moves the given point in the given direction for the given distance.
     * 
     * @param direction_deg a direction in degree, Nemrod::CARDINAL_DIRECTION enum values can be used or any valid degree value
     * @param distance_meter the disance in meter
     * @param point the point to move
     */
    void move_point_in_direction(int direction_deg, int distance_meter, Point& point);
}

#endif	/* GEODATA_HPP */

