#ifndef MPFILE_H
#define	MPFILE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>

namespace Nemrod {

    struct IntPairCompareBySecond {
        bool operator ()(const std::pair<int, int> &a, const std::pair<int, int> &b) {
            return a.second < b.second;
        }
    };
    
    /**
     * Class reprenting the section [IMG ID] of a polish file
     */
    class MpFileHeader {        
    public:
        static MpFileHeader ReadHeader(std::ifstream& fileStream);
        
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

        const std::set<std::pair<int,int>, IntPairCompareBySecond>& GetMapSourceZooms() const {
            return _mapSourceZooms;
        }

        void AddLevelBits(int level, int bits);

        const std::set<std::pair<int,int>, IntPairCompareBySecond>& GetLevelBits() const {
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

        int _lblCoding=-1;
        int _id=-1;
        int _treSize=-1;
        int _rgnLimit=-1;
        int _drawPriority=-1;;
        int _levels=-1;
        
        std::set<std::pair<int,int>, IntPairCompareBySecond> _levelBits;
        std::set<std::pair<int,int>, IntPairCompareBySecond> _mapSourceZooms;

        float _treMargin=-1;

        char _elevation=-1;
        char _preProcess=-1;

        bool _transparent;
        bool _poiIndex;
        bool _poiNumberFirst;
        bool _poiZipFirst;
        
        unsigned char _boolInitStatus = 0x0;
    };
    
    class Polyline {
        
    };
    
    class Polygon {
        
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

        const MpFileHeader& GetHeader() const;
    private:
        MpFileHeader _header;
    };
}

#endif	/* MPFILE_H */


