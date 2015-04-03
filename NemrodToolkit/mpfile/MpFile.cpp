#include "MpFile.hpp"
#include "../Diagnostics.hpp"

#include <string>
#include <iostream>
#include <stdlib.h>
#include "../Utils.hpp"

using namespace Nemrod;

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
        // reaching the end of a section
        [&stopProcessing] (std::string sectionName) {
            if (sectionName == "img id")
                stopProcessing = true;
        },
        // reading a line
        [&fileHeader] (std::string sectionName, std::string line) {
            if (sectionName == "img id") {
                if (starts_with(line, "id="))
                    fileHeader.SetId(Nemrod::to_number(line.substr(3, line.length() - 3)));
                else if (starts_with(line, "name="))
                    fileHeader.SetName(line.substr(5, line.length() - 5));
                else if (starts_with(line, "codepage="))
                    fileHeader.SetCodePage(line.substr(9, line.length() - 9));
                else if (starts_with(line, "levels="))
                    fileHeader.SetLevels(Nemrod::to_number(line.substr(7, line.length() - 7)));
                else if (starts_with(line, "level")) {
                    std::string levelId = line.substr(5, line.length() - line.find_first_of('='));
                    int bits = Nemrod::to_number(line.substr(6 + levelId.length(), line.length() - 6 + levelId.length()));
                    fileHeader.AddLevelBits(Nemrod::to_number(levelId), bits);
                } else if (starts_with(line, "zoom")) {
                    std::string zoomId = line.substr(4, line.length() - line.find_first_of('='));
                    int bits = Nemrod::to_number(line.substr(5 + zoomId.length(), line.length() - 5 + zoomId.length()));
                    fileHeader.AddMapSourceZoom(Nemrod::to_number(zoomId), bits);
                } else if (starts_with(line, "drawpriority="))
                    fileHeader.SetDrawPriority(Nemrod::to_number(line.substr(13, line.length() - 13)));
                else if (starts_with(line, "elevation=") && line.length() > 10)
                    fileHeader.SetElevation(line[10]);
                else if (starts_with(line, "lblCoding="))
                    fileHeader.SetLblCoding(Nemrod::to_number(line.substr(10, line.length() - 10)));
                else if (starts_with(line, "poiindex=") && line.length() > 9)
                    fileHeader.SetPoiIndex(line[9] == 'y');
                else if (starts_with(line, "poinumberfirst=") && line.length() > 15)
                    fileHeader.SetPoiNumberFirst(line[15] == 'y');
                else if (starts_with(line, "poizipfirst=") && line.length() > 12)
                    fileHeader.SetPoiZipFirst(line[12] == 'y');
                else if (starts_with(line, "preprocess=") && line.length() > 11)
                    fileHeader.SetPreProcess(line[11]);
                else if (starts_with(line, "transparent=") && line.length() > 12)
                    fileHeader.SetTransparent(line[12] == 'y');
                else if (starts_with(line, "rgnlimit="))
                    fileHeader.SetRgnLimit(Nemrod::to_number(line.substr(9, line.length() - 9)));
                else if (starts_with(line, "tremargin=") && line.length() > 12)
                    fileHeader.SetTreMargin(Nemrod::to_number(line.substr(10, line.length() - 10)));
                else if (starts_with(line, "tresize="))
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
    
    std::cout << "EOF " << fileStream.eof() << std::endl;
    std::cout << "F1 " << fileStream.fail() << std::endl;
    fileStream.seekg(0);
    std::cout << "F2 " << fileStream.fail() << std::endl;
    polish_file_reader(fileStream, NULL,
            // reaching the end of a section
            [] (std::string sectionName) {
            },
            // reading a line
            [] (std::string sectionName, std::string lineRead) {
                /*if(sectionName == "Project") {
                    if(lineRead.compare(0, 8,"Product=") == 0)
                        projectFile._product = lineRead.substr(8,lineRead.length() - 8);
                     else if(lineRead.compare(0, 10, "Copyright=") == 0)            
                        projectFile._copyright = lineRead.substr(10,lineRead.length() - 10);
                } else if (sectionName == "IMG")
                    projectFile._imgs.push_back(lineRead);*/
            },
            // shouldContinue
            []() {
                return true;
            }
    );
    return mpFile;
}




