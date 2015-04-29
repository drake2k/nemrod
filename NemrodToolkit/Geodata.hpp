#ifndef GEODATA_HPP
#define	GEODATA_HPP

namespace Nemrod{
    
    /**
     * A Geodesic point using latitude and longitude expressed in degrees
     */
    class Point {
    public:
        
        /**
         * Constructs a point using the given latitude and longitude
         * 
         * @param lat Must be >=-90 && <= 90
         * @param longi Must be >=-180 && <= 180
         */
        Point(float lat, float longi) {
            this->SetLatitude(lat);
            this->SetLongitude(longi);
        }

        /**
         * bool operator checks if this instance is set
         * 
         * @return true if both latitude and longitude are set
         */
        operator bool() const {
            return _latitude != 91 && _longitude != -181;
        }
        
        /**
         * Compares two points by longitude and latitude
         * 
         * @param p Point to compare to
         * @return true if they are at the same location
         */
        bool operator==(const Point& p) const{
            return p._latitude == this->_latitude && p._longitude == this->_longitude;
        }
        
        /**
         * Compares two points by longitude and latitude
         * 
         * @param p Point to compare to
         * @return true if they are not the same location
         */
        bool operator!=(const Point& p) const{
            return !(p==*this);
        }

        /**
         * Longitude of the point in degrees
         * @param _longitude Must be >=-180 && <= 180
         */
        void SetLongitude(float _longitude) {
            if (_longitude >= -180 && _longitude <= 180)
                this->_longitude = _longitude;
        }

        /**
         * Longitude of the point in degrees
         * @return Longitude of the point in degrees
         */
        float GetLongitude() const {
            return _longitude;
        }

        /**
         * Latitude of the point in degrees
         * @param _latitude Must be >=-90 && <= 90
         */
        void SetLatitude(float _latitude) {
            if (_latitude >= -90 && _latitude <= 90)
                this->_latitude = _latitude;
        }

        /**
         * Latitude of the point in degrees
         * @return Latitude of the point in degrees
         */
        float GetLatitude() const {
            return _latitude;
        }

    private:
        /*N=90 S=-90*/
        float _latitude = -91;
        /*W=-90 E=90*/
        float _longitude = -181;

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
    
    // formulas: http://www.movable-type.co.uk/scripts/latlong.html
    
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

