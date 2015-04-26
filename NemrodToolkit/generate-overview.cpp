#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <limits>

#include "mpfile/MpFile.hpp"
#include "mapTk/ProjectFile.hpp"
#include "Diagnostics.hpp"
#include "Utils.hpp"

using namespace Nemrod;
using namespace Nemrod::MapTk;

void usage() {
    std::cout << "Usage: " << std::endl;
    // todo
    exit(1);
}

int main(int argc, char** argv) {
    // getcwd 255 magic number might be unsafe, but PATH_MAX is undefined when there is no limits
    TRACE("Current working directory: " << getcwd(NULL, 255))
    
    std::string outputFileName, workingDir;

    if ((argc <= 1) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-')) 
        EXIT_WITH_MSG("No target file name was given, command must end with target file name for the overview map. Use -h for help.");
    else 
        outputFileName = argv[argc-1];
    
    opterr = 0; // suppress getopt errors
    
    std::string projectFileArg;
    char opt;
    while((opt = getopt(argc -1, argv, "p:")) != -1) {
        if(optind >= argc)
            break;
        switch(opt){
            case 'p':
                projectFileArg = optarg;
                break;
            case '?':
                usage();
                break;
        }
    }
    
    if(projectFileArg.empty())
        usage();
    else
        std::cout << "Using ProjectFile: " << projectFileArg << std::endl;
    
    workingDir = projectFileArg.substr(0, projectFileArg.find_last_of("/\\"));
    
    std::cout << "Generating overview map: " << outputFileName << std::endl << std::endl;
    
    ProjectFile projectFile = ProjectFile::LoadProjectFile(projectFileArg);
    
    MpFile overviewMap;
    
    // header 
    overviewMap.GetHeader().SetName(projectFile.GetProduct());
    overviewMap.GetHeader().SetId(projectFile.GetFamilyID());
    overviewMap.GetHeader().SetCopyright(projectFile.GetCopyright());    
    overviewMap.GetHeader().SetPreview(true);
    
    // for these maybe they should be parametrable or rely on IMGs parsed further on
    overviewMap.GetHeader().SetCodePage("1252");
    overviewMap.GetHeader().SetElevation('M');
    overviewMap.GetHeader().SetLblCoding(9);
    overviewMap.GetHeader().SetLevels(2); // this should be removed and read/write from the size of the levelbits set
    overviewMap.GetHeader().GetLevelBits().insert(std::pair<int,int>(0,18));
    overviewMap.GetHeader().GetLevelBits().insert(std::pair<int,int>(1,16));
    // end of header

    // body of the map
    float globalMinLat = std::numeric_limits<float>::max(),
          globalMaxLat = std::numeric_limits<float>::lowest(), 
          globalMinLong = std::numeric_limits<float>::max(), 
          globalMaxLong = std::numeric_limits<float>::lowest();
    
    for(auto &it : projectFile.GetImgs()) {
        std::cout << std::endl << "Loading: " << it << std::endl;        
        MpFile mpFile = MpFile::LoadMPFile(workingDir + "\\" + it + ".mp", false);
        std::cout << it << " loaded." << std::endl;
        
        if(mpFile.GetHeader().IsPreview()) {
            std::cout << it << " is an overview map, skipping." << std::endl;
            continue;
        }
        
        float minLat = std::numeric_limits<float>::max(),
              maxLat = std::numeric_limits<float>::lowest(), 
              minLong = std::numeric_limits<float>::max(), 
              maxLong = std::numeric_limits<float>::lowest();
        
        get_shapes_max_extents(mpFile.GetPolylines(), minLat, maxLat, minLong, maxLong);
        get_shapes_max_extents(mpFile.GetPolygons(), minLat, maxLat, minLong, maxLong);
        
        // update Area of coverage points (0x4b polygon)
        if(maxLong > globalMaxLong)
            globalMaxLong = maxLong;                        
        if(minLong < globalMinLong)
            globalMinLong = minLong;
        if(maxLat > globalMaxLat)
            globalMaxLat = maxLat;
        if(minLat < globalMinLat)
            globalMinLat = minLat;
        
        // generate 4 points that will represent the extents this map's Area of selection (0x4a polygon)
        Point topLeft(minLat, maxLong),
              botLeft(minLat, minLong),
              topRight(maxLat, maxLong),
              botRight(maxLat, minLong);
        
        // create the area of map selection polygon and add to overview map
        Polygon areaMapSelection;
        areaMapSelection.SetTypeCode(0x4a);                               // ~[0x1d] means that what follows is an abbreviation
        areaMapSelection.SetLabel(mpFile.GetHeader().GetName() + " - 10000000~[0x1d]10000000");
        std::vector<Point> mapExtents;
        mapExtents.push_back(topLeft);
        mapExtents.push_back(botLeft);
        mapExtents.push_back(topRight);
        mapExtents.push_back(botRight);
        
        areaMapSelection.AddPoints(0, mapExtents);
        overviewMap.GetPolygons().push_back(areaMapSelection);
    }
    // add Area of coverage (background) Polygon 0x4b ?
    
    // write file
    
    
    std::cout << std::endl << "Generated overview map: " << outputFileName << std::endl;
    
#ifdef DEBUG
    std::cout << "DEBUG Build, press ENTER to continue" << std::endl;
    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
#endif
    return 0;
}



