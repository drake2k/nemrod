#ifndef MPFILE_H
#define	MPFILE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>

namespace Nemrod {

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

        void SetLatitude(float _latitude) {
            if (_latitude >= -90 && _latitude <= 90)
                this->_latitude = _latitude;
        }

        float GetLatitude() const {
            return _latitude;
        }

    private:
        /*N=90 S=-90*/
        float _latitude = -91;
        /*W=-90 E=90*/
        float _longitude = -91;

    };
    
    struct IntPairCompareByFirst {
        bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.first < b.first;
        }
    };
    
    struct IntPointVectorPairCompareByInt {
        bool operator()(const std::pair<int, std::vector<Point>>& a, const std::pair<int, std::vector<Point>>& b) {
            return a.first < b.first;
        }
    };
    
    
    
    /**
     * When implementing writing this class' method will be abstract
     * 
     */
    class Shape {
    public:
        Shape() : _points() {} ;
        virtual ~Shape() {}
        void AddPoints(int level, const std::vector<Point>&);
        
        const std::set<std::pair<int, std::vector<Point>>, IntPointVectorPairCompareByInt>& GetPoints() const {
            return _points;
        }

        void SetEndLevel(int _endLevel) {
            this->_endLevel = _endLevel;
        }

        int GetEndLevel() const {
            return _endLevel;
        }

        void SetLabel(std::string _label) {
            this->_label = _label;
        }

        std::string GetLabel() const {
            return _label;
        }

        void SetTypeCode(short _typeCode) {
            this->_typeCode = _typeCode;
        }

        short GetTypeCode() const {
            return _typeCode;
        }
    private:
        short _typeCode = 0;
        std::string _label = "";
        int _endLevel = -1;
        
        // todo this could as well be a map since it doesnt need to be ordered by the key (level)
        std::set<std::pair<int, std::vector<Point>>, IntPointVectorPairCompareByInt> _points ;
    };

    class Polyline : public Shape {
    public:
    private:
    };

    class Polygon : public Shape {
    
    public:
        void SetBackground(bool _background) {
            this->_background = _background;
        }

        bool IsBackground() const {
            return _background;
        }
        
    private:
        bool _background = false;
    };

    /**
     * Class reprenting the section [IMG ID] of a polish file
     */
    class MpFileHeader {
    public:
        
        /**
         * Reads the MpFileHeader at the given filestream
         * 
         * @param fileStream filestream positioned at the [IMG ID] section
         * @return A MpFileHeader object
         */
        static MpFileHeader ReadHeader(std::ifstream& fileStream);
        
        /**
         * Writes the header using the given fileStream at the current position
         * @param fileStream the filestream to write the MpFileHeader to
         */
        void WriteHeader(std::ofstream& fileStream);

        /**
         * BITMASK Flag indicating the transparent property is initialized
         */
        static const unsigned char TRANSPARENT_INITIALIZED = 0x1;
        /**
         * BITMASK Flag indicating the poiindex property is initialized
         */
        static const unsigned char POIINDEX_INITIALIZED = 0x2;
        /**
         * BITMASK Flag indicating the poinumberfirst property is initialized
         */
        static const unsigned char POINUMBERFIRST_INITIALIZED = 0x4;
        /**
         * BITMASK Flag indicating the poizipfirst property is initialized
         */
        static const unsigned char POIZIPFIRST_INITIALIZED = 0x8;        
        /**
         * BITMASK Flag indicating the preview property is initialized
         */
        static const unsigned char PREVIEW_INITIALIZED = 0x10;

        void SetPoiZipFirst(bool _poiZipFirst) {
            this->_poiZipFirst = _poiZipFirst;
            this->_boolInitStatus = this->_boolInitStatus | POIZIPFIRST_INITIALIZED;
        }

        bool IsPoiZipFirst() const {
            return _poiZipFirst;
        }

        void SetPoiNumberFirst(bool _poiNumberFirst) {
            this->_poiNumberFirst = _poiNumberFirst;
            this->_boolInitStatus = this->_boolInitStatus | POINUMBERFIRST_INITIALIZED;
        }

        bool IsPoiNumberFirst() const {
            return _poiNumberFirst;
        }

        void SetPoiIndex(bool _poiIndex) {
            this->_poiIndex = _poiIndex;
            this->_boolInitStatus = this->_boolInitStatus | POIINDEX_INITIALIZED;
        }

        bool IsPoiIndex() const {
            return _poiIndex;
        }

        void SetTransparent(bool _transparent) {
            this->_transparent = _transparent;
            this->_boolInitStatus = this->_boolInitStatus | TRANSPARENT_INITIALIZED;
        }

        bool IsTransparent() const {
            return _transparent;
        }
        
        void SetPreview(bool _preview) {
            this->_preview = _preview;
            this->_boolInitStatus = this->_boolInitStatus | PREVIEW_INITIALIZED;
        }

        bool IsPreview() const {
            return _preview;
        }

        void SetPreProcess(char _preProcess) {
            this->_preProcess = _preProcess;
        }

        char GetPreProcess() const {
            return _preProcess;
        }

        void SetElevation(char _elevation) {
            this->_elevation = _elevation;
        }

        char GetElevation() const {
            return _elevation;
        }

        void SetTreMargin(float _treMargin) {
            this->_treMargin = _treMargin;
        }

        float GetTreMargin() const {
            return _treMargin;
        }

        void AddMapSourceZoom(int level, int bits);

        const std::set<std::pair<int, int>, IntPairCompareByFirst>& GetMapSourceZooms() const {
            return _mapSourceZooms;
        }

        void AddLevelBits(int level, int bits);

        std::set<std::pair<int, int>, IntPairCompareByFirst>& GetLevelBits() {
            return _levelBits;
        }

        void SetLevels(int _levels) {
            this->_levels = _levels;
        }

        int GetLevels() const {
            return _levels;
        }

        void SetDrawPriority(int _drawPriority) {
            this->_drawPriority = _drawPriority;
        }

        int GetDrawPriority() const {
            return _drawPriority;
        }

        void SetRgnLimit(int _rgnLimit) {
            this->_rgnLimit = _rgnLimit;
        }

        int GetRgnLimit() const {
            return _rgnLimit;
        }

        void SetTreSize(int _treSize) {
            this->_treSize = _treSize;
        }

        int GetTreSize() const {
            return _treSize;
        }

        void SetId(int _id) {
            this->_id = _id;
        }

        int GetId() const {
            return _id;
        }

        void SetLblCoding(int _lblCoding) {
            this->_lblCoding = _lblCoding;
        }

        int GetLblCoding() const {
            return _lblCoding;
        }

        void SetName(std::string _name) {
            this->_name = _name;
        }

        std::string GetName() const {
            return _name;
        }

        void SetCodePage(std::string _codePage) {
            this->_codePage = _codePage;
        }

        std::string GetCodePage() const {
            return _codePage;
        }
        
        void SetCopyright(std::string _copyright) {
            this->_copyright = _copyright;
        }

        std::string GetCopyright() const {
            return _copyright;
        }

        /**
         * Usefull when writing, if a property has not been initialized it will not be writen to the file.
         * 
         * @return bool init status
         */
        unsigned char GetBoolInitStatus() const {
            return _boolInitStatus;
        }
    private:
        std::string _codePage;
        std::string _name;
        std::string _copyright;

        int _lblCoding = -1;
        int _id = -1;
        int _treSize = -1;
        int _rgnLimit = -1;
        int _drawPriority = -1;
        // todo, this should be removed and read/write from the size of the levelbits set
        int _levels = -1;

        std::set<std::pair<int, int>, IntPairCompareByFirst> _levelBits;
        std::set<std::pair<int, int>, IntPairCompareByFirst> _mapSourceZooms;

        float _treMargin = -1;

        char _elevation = -1;
        char _preProcess = -1;

        bool _transparent = false;
        bool _poiIndex = false;
        bool _poiNumberFirst = false;
        bool _poiZipFirst = false;
        bool _preview = false;

        unsigned char _boolInitStatus = 0x0;
    };

    /**
     * Class representing a polish file 
     * 
     */
    class MpFile {
    public:
        /**
         * Loads a polish file and returns the MpFile instance
         * @param fileName The full filepath to the file to be loaded
         * @param onlyHeader Defaults to false, if true will only load the header, attempts at reading other 
         * fields than the MpfileHeader referenced returned by GetHeader() will fail.
         * @return MpFile instance
         */
        static MpFile LoadMPFile(std::string fileName, bool onlyHeader = false);
        
        /**
         * Writes the object into a MpFile to the given filename
         * @param fileName where to write the object
         */
        void WriteMPFile(std::string fileName);

        MpFileHeader& GetHeader() {
            return _header;
        }
        
        std::vector<Polygon>& GetPolygons() {
            return _polygons;
        }
        
        std::vector<Polyline>& GetPolylines() {
            return _polylines;
        }
        
        void PrintSizes() {
            std::cout << "Polygons: " << _polygons.size() << " Polylines: " << _polylines.size() << std::endl;
        }
        
        
    private:
        MpFileHeader _header;
        std::vector<Polygon> _polygons;
        std::vector<Polyline> _polylines;
    };
}

#endif	/* MPFILE_H */


