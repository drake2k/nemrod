#include "MpFile.hpp"
#include "../Diagnostics.hpp"

#include <string>
#include <iostream>
#include <stdlib.h>
#include "../Utils.hpp"

using namespace Nemrod;

void MpFileHeader::AddLevelBits(int level, int bits) {
    
}

void MpFileHeader::AddMapSourceZoom(int level, int bits) {

}

MpFile MpFile::LoadMPFile(std::string fileName, bool onlyHeader) {
    return MpFile();
}

MpFileHeader MpFileHeader::ReadHeader(std::ifstream fileStream) {
    MpFileHeader fileHeader;
    std::string line, lineSectionName;
    while(std::getline(fileStream, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        if(line[0] == '[' && (line.length() > 2 && line[line.length()-1] == ']')) {
            lineSectionName  = line.substr(1, line.length()-2);
            if(lineSectionName != "END")
                EXIT_WITH_MSG("MpFile parsing error, overlapping sections: " + lineSectionName + " within " + "[IMG ID]");
            else
                break;
        }
        
        if(starts_with(line, "Id="))
            fileHeader.SetId(Nemrod::to_number(line.substr(3, line.length() - 3)));
        else if(starts_with(line, "Name="))
            fileHeader.SetName(line.substr(5, line.length() - 5));
        else if(starts_with(line, "Codepage="))
            fileHeader.SetCodePage(line.substr(9, line.length() - 9));
        else if(starts_with(line, "Levels="))
            fileHeader.SetLevels(Nemrod::to_number(line.substr(7, line.length() - 7)));
        else if(starts_with(line, "Level"))
        {
            std::string levelId = line.substr(5, line.length() - line.find_first_of('='));
            int bits = Nemrod::to_number(line.substr(6+levelId.length(), line.length() - 6+levelId.length()));
            fileHeader.AddLevelBits(Nemrod::to_number(levelId), bits);
        }
        else if(starts_with(line, "Zoom"))
        {
            std::string zoomId = line.substr(4, line.length() - line.find_first_of('='));
            int bits = Nemrod::to_number(line.substr(5+zoomId.length(), line.length() - 5+zoomId.length()));
            fileHeader.AddMapSourceZoom(Nemrod::to_number(zoomId), bits);
        }
        else if(starts_with(line, "DrawPriority="))
            fileHeader.SetDrawPriority(Nemrod::to_number(line.substr(13, line.length() - 13)));
        else if(starts_with(line, "Elevation=") && line.length() > 10)
            fileHeader.SetElevation(line[10]);
        else if(starts_with(line, "LblCoding="))
            fileHeader.SetLblCoding(Nemrod::to_number(line.substr(10, line.length() - 10)));
        else if(starts_with(line, "POIIndex=") && line.length() > 9)
            fileHeader.SetPoiIndex(line[9] == 'Y');
        else if(starts_with(line, "POINumberFirst=") && line.length() > 15)
            fileHeader.SetPoiNumberFirst(line[15] == 'Y');
        else if(starts_with(line, "POIZipFirst=") && line.length() > 12)
            fileHeader.SetPoiZipFirst(line[12] == 'Y');
        else if(starts_with(line, "Preprocess=") && line.length() > 11)
            fileHeader.SetPreProcess(line[11]);
        else if(starts_with(line, "Transparent=") && line.length() > 12)
            fileHeader.SetTransparent(line[12] == 'Y');
        else if(starts_with(line, "RgnLimit="))
            fileHeader.SetRgnLimit(Nemrod::to_number(line.substr(9, line.length() - 9)));
        else if(starts_with(line, "TreMargin=") && line.length() > 12)
            fileHeader.SetTreMargin(Nemrod::to_number(line.substr(10, line.length() - 10)));
        else if(starts_with(line, "TreSize="))
            fileHeader.SetTreSize(Nemrod::to_number(line.substr(8, line.length() - 8)));
    }
    return fileHeader;
}


