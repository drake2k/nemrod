#include "MpFile.hpp"
#include "../Diagnostics.hpp"
#include "../Utils.hpp"

#include <string>
#include <iostream>
#include <ios>
#include <stdlib.h>
#include <iterator>
#include <memory>
//#include <regex>

using namespace Nemrod;

void Shape::AddPoints(int level, const std::vector<Point>& points){
    this->_points.insert(std::pair<int,const std::vector<Point>&>(level, points));
}

void MpFileHeader::AddLevelBits(int level, int bits) {
    this->_levelBits.insert(std::pair<int,int>(level, bits));
}

void MpFileHeader::AddMapSourceZoom(int level, int mapSourceRange) {
    this->_mapSourceZooms.insert(std::pair<int,int>(level, mapSourceRange));
}

MpFileHeader MpFileHeader::ReadHeader(std::ifstream& fileStream) {
    MpFileHeader fileHeader;
    
    bool stopProcessing = false;
    polish_file_reader(fileStream, NULL,
        // reached the start of a section
        NULL,
        // reaching the end of a section
        [&stopProcessing] (std::string sectionName) {
            if (sectionName == "img id")
                stopProcessing = true;
        },
        // reading a line
        [&fileHeader] (std::string sectionName, std::string line) {
            if (sectionName == "img id") {
                if (starts_with(line, "id=", false) && line.length()>3)
                    fileHeader.SetId(line.substr(3, line.length() - 3));
                else if (starts_with(line, "name=", false))
                    fileHeader.SetName(line.substr(5, line.length() - 5));
                else if (starts_with(line, "codepage=", false))
                    fileHeader.SetCodePage(line.substr(9, line.length() - 9));
                else if (starts_with(line, "copyright=", false))
                    fileHeader.SetCopyright(line.substr(10, line.length() - 10));
                else if (starts_with(line, "levels=", false))
                    fileHeader.SetLevels(Nemrod::to_number(line.substr(7, line.length() - 7)));
                else if (starts_with(line, "level", false) && line.length()>5) {
                    std::string levelId = line.substr(5, line.find_first_of('=') - 5);
                    int bits = Nemrod::to_number(line.substr(6 + levelId.length(), line.length() - 6 - levelId.length()));
                    fileHeader.AddLevelBits(Nemrod::to_number(levelId), bits);
                } else if (starts_with(line, "zoom", false) && line.length()>4) {
                    std::string zoomId = line.substr(4, line.find_first_of('=') - 4);
                    int bits = Nemrod::to_number(line.substr(5 + zoomId.length(), line.length() - 5 - zoomId.length()));
                    fileHeader.AddMapSourceZoom(Nemrod::to_number(zoomId), bits);
                } else if (starts_with(line, "drawpriority=", false)&& line.length() > 13)
                    fileHeader.SetDrawPriority(Nemrod::to_number(line.substr(13, line.length() - 13)));
                else if (starts_with(line, "elevation=", false) && line.length() > 10)
                    fileHeader.SetElevation(line[10]);
                else if (starts_with(line, "lblCoding=", false) && line.length() > 10)
                    fileHeader.SetLblCoding(Nemrod::to_number(line.substr(10, line.length() - 10)));
                else if (starts_with(line, "poiindex=", false) && line.length() > 9)
                    fileHeader.SetPoiIndex(tolower(line[9]) == 'y');
                else if (starts_with(line, "poinumberfirst=", false) && line.length() > 15)
                    fileHeader.SetPoiNumberFirst(tolower(line[15]) == 'y');
                else if (starts_with(line, "poizipfirst=", false) && line.length() > 12)
                    fileHeader.SetPoiZipFirst(tolower(line[12]) == 'y');
                else if (starts_with(line, "preprocess=", false) && line.length() > 11)
                    fileHeader.SetPreProcess(line[11]);
                else if (starts_with(line, "transparent=", false) && line.length() > 12)
                    fileHeader.SetTransparent(tolower(line[12]) == 'y');
                else if (starts_with(line, "preview=", false) && line.length() > 8)
                    fileHeader.SetPreview(tolower(line[8]) == 'y');
                else if (starts_with(line, "rgnlimit=", false) && line.length()>9)
                    fileHeader.SetRgnLimit(Nemrod::to_number(line.substr(9, line.length() - 9)));
                else if (starts_with(line, "tremargin=", false) && line.length() > 10)
                    fileHeader.SetTreMargin(Nemrod::to_number(line.substr(10, line.length() - 10)));
                else if (starts_with(line, "tresize=", false) && line.length() > 8)
                    fileHeader.SetTreSize(Nemrod::to_number(line.substr(8, line.length() - 8)));
            }
        },
        // shouldContinue
        [&stopProcessing]() {
            return !stopProcessing;
        }
    );
    return fileHeader;
}

MpFile MpFile::LoadMPFile(std::string fileName, bool onlyHeader) {
    std::ifstream fileStream(fileName);
    if(!fileStream.is_open())
        EXIT_WITH_MSG("MP file: " + fileName + " does not exists or failed to open");
    
    MpFile mpFile;
    mpFile._header = MpFileHeader::ReadHeader(fileStream);
    if(onlyHeader)
        return mpFile;
    
    fileStream.seekg(0);
    
    Shape* currentShape;
    
    /*
    std::unique_ptr<Shape> testPtr;
    auto testPoly = dynamic_unique_ptr_cast<Polygon, Shape>(std::move(testPtr));
    */
    polish_file_reader(fileStream, NULL,
        // reaching the start of a section
        [&currentShape] (std::string sectionName) {
            if(sectionName == "polygon") {
                currentShape = new Polygon();
                TRACE("Created polygon @" << currentShape)
            }
            else if(sectionName == "polyline") {
                currentShape = new Polyline();
                TRACE("Created polyline @" << currentShape)
            }
        },
        // reaching the end of a section
        [&currentShape, &mpFile] (std::string sectionName) {
            if(sectionName == "polygon" || sectionName == "polyline") {
                if(sectionName == "polygon")
                    mpFile._polygons.push_back(*dynamic_cast<Polygon*>(currentShape));
                else
                    mpFile._polylines.push_back(*dynamic_cast<Polyline*>(currentShape));
                delete currentShape;
                TRACE("Deleted shape @" << currentShape)
            }
        },
        // reading a line
        [&currentShape, &mpFile] (std::string sectionName, std::string lineRead) {
            if(sectionName == "polygon" || sectionName == "polyline") {
                if(starts_with(lineRead, "background=", false) && lineRead.length() > 11 && sectionName=="polygon") { // if "polygon" make the cast somewhat safer
                    dynamic_cast<Polygon*>(currentShape)->SetBackground('y' == tolower(lineRead[11]));
                } else if(starts_with(lineRead,"type=", false) && lineRead.length() > 5)
                    currentShape->SetTypeCode(Nemrod::to_number_from_hex(lineRead.substr(5, lineRead.length() - 5)));
                else if(starts_with(lineRead,"label=", false) && lineRead.length() > 6)
                    currentShape->SetLabel(lineRead.substr(6, lineRead.length() - 6));
                else if(starts_with(lineRead,"endlevel=", false) && lineRead.length() > 9)
                    currentShape->SetEndLevel(Nemrod::to_number(lineRead.substr(9, lineRead.length() - 9)));
                else if(starts_with(lineRead, "data", false)){
                    std::string level = lineRead.substr(4, lineRead.find_first_of('=') - 4);
                    std::string pointsString = lineRead.substr(5 + level.length(), lineRead.length() - 5 - level.length());

                    // lat always first
                    //float latitude = -2, longitude = -2;
                    int state = 0; //0 nowhere, 1 reading lat, 2 reading long
                    std::stringstream latBuffer, longBuffer;
                    std::vector<Point> points;
                    
                    for(auto i = pointsString.begin(), pointsStringEnd = pointsString.end(); i < pointsStringEnd; i++) {
                        switch(state) {
                            case 0:
                                if(*i == '(')
                                    state = 1;
                                break;
                            case 1:
                                if(*i == ',')
                                    state = 2;
                                else {
                                    // check numeric or . and fill lat buffer
                                    if(isdigit(*i) || *i == '-' || *i == '.')
                                        latBuffer << *i;
                                }
                                break;
                            case 2:
                                if(*i != ')') {
                                    // check numeric or . and fill long buffer
                                    if(isdigit(*i) || *i == '-' || *i == '.')
                                        longBuffer << *i;
                                } else {
                                    // flush buffers in vector
                                    state = 0;
                                    points.push_back(Point(Nemrod::to_float(latBuffer.str()),Nemrod::to_float(longBuffer.str())));
                                    
                                    latBuffer.str("");
                                    latBuffer.clear();
                                    longBuffer.str("");
                                    longBuffer.clear();
                                }
                                break;  
                        }
                    }
                    TRACE("Adding " << points.size() << " points to shape.")
                    currentShape->AddPoints(Nemrod::to_number(level), points);
                    
                    /*
                    if we get gcc4.9 support 
                    const std::string regexString = "(\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)\\s*(,|\\z|,\\z))";
                    
                    std::regex pointsRegex(regexString);
                    auto pointsStart = 
                        std::sregex_iterator(pointsString.begin(), pointsString.end(), pointsRegex);
                    auto pointsEnd = std::sregex_iterator();
                    
                    for (std::sregex_iterator i = pointsStart; i != pointsEnd; ++i) {
                        std::smatch match = *i;
                        
                    }*/
                }
            }
        },
        // shouldContinue
        NULL
    );
    fileStream.close();
    mpFile.PrintSizes();
    return mpFile;
}

void MpFileHeader::WriteHeader(std::ofstream& fileStream) {
    fileStream << "[IMG ID]" << std::endl;
    
    // simple fields
    fileStream << "Name=" << _name << std::endl;
    fileStream << "ID=" << _id << std::endl;
    fileStream << "Codepage=" << _codePage << std::endl;
    fileStream << "Copyright=" << _copyright << std::endl;
    fileStream << "DrawPriority=" << _drawPriority << std::endl;
    fileStream << "Elevation=" << _elevation << std::endl;
    fileStream << "LblCoding=" << _lblCoding << std::endl;
    fileStream << "Preprocess=" << _preProcess << std::endl;
    fileStream << "RgnLimit=" << _rgnLimit << std::endl;
    fileStream << "TreMargin=" << _treMargin << std::endl; // todo format at 5 decimal, always even if zeros
    if(_treSize != -1)
        fileStream << "TreSize=" << _treSize << std::endl;
    
    // levels and zooms
    fileStream << "Levels=" << _levels << std::endl;
    for(auto &itLevelBits : _levelBits)
        fileStream << "Level" << itLevelBits.first << "=" << itLevelBits.second << std::endl;
    for(auto &itZooms : _mapSourceZooms)
        fileStream << "Zoom" << itZooms.first << "=" << itZooms.second << std::endl;

    // bools
    if(_boolInitStatus & PREVIEW_INITIALIZED)
        fileStream << "Preview=" << ((_preview)?'Y':'N') << std::endl;
    if(_boolInitStatus & TRANSPARENT_INITIALIZED)
        fileStream << "Transparent=" << ((_transparent)?'Y':'N') << std::endl;
    if(_boolInitStatus & POIINDEX_INITIALIZED)
        fileStream << "POIIndex=" << ((_poiIndex)?'Y':'N') << std::endl;
    if(_boolInitStatus & POINUMBERFIRST_INITIALIZED)
        fileStream << "POINumberFirst=" << ((_poiNumberFirst)?'Y':'N') << std::endl;
    if(_boolInitStatus & POIZIPFIRST_INITIALIZED)
        fileStream << "POIZipFirst=" << ((_poiZipFirst)?'Y':'N') << std::endl;
    
    fileStream << "[END-IMG ID]" << std::endl << std::endl;
}

void WriteShape(Shape& shape, std::ofstream& fileStream){
    fileStream << "Type=" << to_hex_from_number(shape.GetTypeCode()) << std::endl;
        fileStream << "Label=" << shape.GetLabel()<< std::endl;
        if(shape.GetEndLevel() != -1)
            fileStream << "Endlevel=" << shape.GetEndLevel() << std::endl;
        for(auto &itLevels : shape.GetPoints()){
            fileStream << "Data" << itLevels.first << "=";
            for(auto &itDataPoints : itLevels.second) 
                fileStream <<  
                // only prefix with a comma if not first
                ((&(*itLevels.second.begin()) != &itDataPoints)?",(":"(")  << 
                itDataPoints.GetLatitude() << "," << itDataPoints.GetLongitude() << ")";
            fileStream << std::endl;
        }
}

void MpFile::WriteMPFile(std::string fileName) {
    std::ofstream fileStream(fileName);
    fileStream.setf(std::ofstream::fixed);
    fileStream.precision(5);
    if(!fileStream.is_open())
        EXIT_WITH_MSG("Failed opening stream to write @: " + fileName);
    
    // write header
    _header.WriteHeader(fileStream);
    
    // write body
    // polygons
    for(auto &it : _polygons) {
        fileStream << "[POLYGON]" << std::endl;
        // write shape stuff
        WriteShape(it, fileStream);
        // specific to polygon attributes
        if(it.IsBackground())
            fileStream << "Background=Y" << std::endl;
        fileStream << "[END]"<< std::endl << std::endl;
    }
    
    // polylines
    for(auto &it : _polylines) {
        fileStream << "[POLYLINE]" << std::endl;
        // write shape stuff
        WriteShape(it, fileStream);
        fileStream << "[END]"<< std::endl << std::endl;
    }
    fileStream.close();
}

