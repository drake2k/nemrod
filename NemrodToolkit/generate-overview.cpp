#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <limits>
#include <getopt.h>

#include "mpfile/MpFile.hpp"
#include "mapTk/ProjectFile.hpp"
#include "Diagnostics.hpp"
#include "Utils.hpp"
#include "Geodata.hpp"

using namespace Nemrod;
using namespace Nemrod::MapTk;

void help() {
    std::cout << "Usage: generate-overview.exe [OPTIONS] outputFileName" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t" << "-p, --project=projectFile: REQUIRED, Use the given project file" << std::endl;
    std::cout << "\t" << "-r, --coverage: Enables the generation of the Area of Map coverage polygon" << std::endl;
    std::cout << "\t" << "-c, --emc=extendMapCoverageDistance: Extend 0x4a polygon by the given amount of meters" << std::endl;
    std::cout << "\t" << "-s, --ems=extendMapAreaSelectionDistance:  Extend 0x4b polygon by the given amount of meters" << std::endl;    
    std::cout << "\t" << "-h, --help: Shows this message." << std::endl;
    exit(1);
}

int main(int argc, char** argv) {
    // getcwd 255 magic number might be unsafe, but PATH_MAX is undefined when there is no limits
    TRACE("Current working directory(getcwd): " << getcwd(NULL, 255))
    opterr = 0; // suppress getopt errors
    
    std::string projectFileArg;
    int generateAreaOfMapCoveragePoly = false;
    int extendMapCoverageDistance = 0;
    int extendMapAreaSelectionDistance = 0;
    
    static struct option long_options[] =
    {
      /* This option sets a flag. */
      {"coverage", no_argument, &generateAreaOfMapCoveragePoly, 1},
      {"help", no_argument, 0, 'h'},
      {"project", required_argument, 0, 'p'},
      /*extend map coverage distance*/
      {"emc",  required_argument, 0, 'c'},
      /*extend map selection distance*/
      {"ems",  required_argument, 0, 's'},
      {0, 0, 0, 0}
    };
    
    char opt; int option_index = 0;
    while((opt = getopt_long(argc, argv, "rhp:s:c:", long_options, &option_index)) != -1) {
        switch(opt){
            case 0:
                if (long_options[option_index].flag != 0)
                    break;
                // should never get to next line
                std::cout << long_options[option_index].name << " has val " << long_options[option_index].val;
                break;
            case 'p':
                projectFileArg = optarg;
                break;
            case 'c':
                extendMapCoverageDistance = atoi(optarg);
                break;
            case 's':
                extendMapAreaSelectionDistance = atoi(optarg);
                break;
            case 'r':
                generateAreaOfMapCoveragePoly = true;
                break;
            case 'h':
            case '?':
                help();
                break;
        }
    }
    
    std::string outputFileName, projectFileDir;
    
    // todo test if this could replace the previous logic, cleaner and we could also ignore trailing stuff instead of using last one (by making a lookup at optind instead)
    /*if (optind < argc)
        outputFileName = argv[argc-1];
    else 
        EXIT_WITH_MSG("No target file name was given, command must end with target file name for the overview map. Use -h for help.");
    */
    
    if ((argc <= 1) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-')) 
        EXIT_WITH_MSG("No target file name was given, command must end with target file name for the overview map. Use -h for help.");
    else 
        outputFileName = argv[argc-1];
    
    std::cout << std::endl << "Generating overview map: " << outputFileName << std::endl << std::endl;
    
    if(projectFileArg.empty())
        help();
    else
        std::cout << "Using ProjectFile: " << projectFileArg << std::endl;
    
    if(projectFileArg.find_last_of("/\\") != std::string::npos)
        projectFileDir = projectFileArg.substr(0, projectFileArg.find_last_of("/\\"));
    else
        projectFileDir="";
    
    ProjectFile projectFile = ProjectFile::LoadProjectFile(projectFileArg);
    
    MpFile overviewMap;
    // header 
    overviewMap.GetHeader().SetName(projectFile.GetProduct());
    overviewMap.GetHeader().SetId(projectFile.GetOverviewMapName().substr(0, projectFile.GetOverviewMapName().length()-4));
    overviewMap.GetHeader().SetCopyright(projectFile.GetCopyright());    
    overviewMap.GetHeader().SetPreview(true);
    
    // for these maybe they should be parametrable or rely on IMGs parsed further on
    overviewMap.GetHeader().SetCodePage("1252");
    overviewMap.GetHeader().SetElevation('M');
    overviewMap.GetHeader().SetLblCoding(9);
    
    // end of header

    // body of the map
    float globalMinLat = std::numeric_limits<float>::max(),
          globalMaxLat = std::numeric_limits<float>::lowest(), 
          globalMinLong = std::numeric_limits<float>::max(), 
          globalMaxLong = std::numeric_limits<float>::lowest();
    int highestDetailledLevelBits = 0, lowestDetailledLevelBits = std::numeric_limits<int>::max(), imgCount = 0;
    
    for(auto &it : projectFile.GetImgs()) {
        std::cout << std::endl << "Loading: " << it << std::endl;        
        MpFile mpFile = MpFile::LoadMPFile(projectFileDir + ((projectFileDir != "")?"\\":"") + it + ".mp", false);
        std::cout << it << " loaded." << std::endl;
        
        if(mpFile.GetHeader().IsPreview()) {
            std::cout << it << " is an overview map, skipping." << std::endl;
            continue;
        }
        imgCount ++;
        if(!mpFile.GetHeader().GetLevelBits().empty()) {
            if((*mpFile.GetHeader().GetLevelBits().begin()).second > highestDetailledLevelBits)
                highestDetailledLevelBits = (*mpFile.GetHeader().GetLevelBits().begin()).second;

            if((*--mpFile.GetHeader().GetLevelBits().end()).second < lowestDetailledLevelBits)
                lowestDetailledLevelBits = (*--mpFile.GetHeader().GetLevelBits().end()).second;
        }
        
        float minLat = std::numeric_limits<float>::max(),
              maxLat = std::numeric_limits<float>::lowest(), 
              minLong = std::numeric_limits<float>::max(), 
              maxLong = std::numeric_limits<float>::lowest();
        
        compute_shapes_max_extents(mpFile.GetPolylines(), minLat, maxLat, minLong, maxLong);
        compute_shapes_max_extents(mpFile.GetPolygons(), minLat, maxLat, minLong, maxLong);
        
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
        Point topLeft(maxLat, minLong),
              botLeft(minLat, minLong),
              topRight(maxLat, maxLong),
              botRight(minLat, maxLong);
        
        // check if we extend map area selection distance, usefull for transparent maps
        if(extendMapAreaSelectionDistance > 0) {
            move_point_in_direction(Nemrod::NORTH_WEST, extendMapAreaSelectionDistance, topLeft);
            move_point_in_direction(Nemrod::NORTH_EAST, extendMapAreaSelectionDistance, topRight);
            move_point_in_direction(Nemrod::SOUTH_WEST, extendMapAreaSelectionDistance, botLeft);
            move_point_in_direction(Nemrod::SOUTH_EAST, extendMapAreaSelectionDistance, botRight);
        }
        
        // create the area of map selection polygon and add to overview map
        Polygon areaMapSelection;
        areaMapSelection.SetTypeCode(0x4a);                               // ~[0x1d] means that what follows is an abbreviation
        areaMapSelection.SetLabel(mpFile.GetHeader().GetName() + " - " + mpFile.GetHeader().GetId() + "~[0x1d]" + mpFile.GetHeader().GetId());
        areaMapSelection.AddPoints(0, {topLeft, topRight, botRight, botLeft});
        overviewMap.GetPolygons().push_back(areaMapSelection);
    }
    
    // overview should have 2 levels
    overviewMap.GetHeader().SetLevels(2); // this should be removed and read/write from the size of the levelbits set
    
    // Now set the level details, level 0 will be the highest detail found in IMGs, level one is fixed to 16
    // cGpsMapper spec says that the highest detailled level of the overview map should be lower 
    // than the lowest level of the tiles (img). 
    
    // However this has the side effect that the tiles is offset since the coordinates precision is offset. In some cases
    // this is not an issue but if you have a single tile, it looks weird
    
    // via expirimentation, I found that with 1 tile, using the highest possible level of details from the tile in the overview
    // works perfectly, but with more than 1 tile it creates a mess. Therefore check imgCount and apply the correct logic
    
    if(imgCount == 0) {
        overviewMap.GetHeader().AddLevelBits(0,highestDetailledLevelBits);
        overviewMap.GetHeader().AddLevelBits(1,highestDetailledLevelBits > 16 ? 16 : highestDetailledLevelBits-1);
    } else {
        overviewMap.GetHeader().AddLevelBits(0,lowestDetailledLevelBits-1);
        overviewMap.GetHeader().AddLevelBits(1,lowestDetailledLevelBits-2);
    }
    
    if(generateAreaOfMapCoveragePoly) {
        Point topLeft(globalMaxLat, globalMinLong),
              botLeft(globalMinLat, globalMinLong),
              topRight(globalMaxLat, globalMaxLong),
              botRight(globalMinLat, globalMaxLong);
        
         // check if we extend map coverage area distance, usefull for transparent maps
        if(extendMapCoverageDistance > 0) {
            move_point_in_direction(Nemrod::NORTH_WEST, extendMapCoverageDistance, topLeft);
            move_point_in_direction(Nemrod::NORTH_EAST, extendMapCoverageDistance, topRight);
            move_point_in_direction(Nemrod::SOUTH_WEST, extendMapCoverageDistance, botLeft);
            move_point_in_direction(Nemrod::SOUTH_EAST, extendMapCoverageDistance, botRight);
        }
        
        // create the  Area of coverage polygon and add to overview map
        Polygon areaMapCoverage;
        areaMapCoverage.SetTypeCode(0x4b);
        areaMapCoverage.SetLabel(projectFile.GetProduct());
        areaMapCoverage.AddPoints(0, {topLeft, topRight, botRight, botLeft});
        overviewMap.GetPolygons().push_back(areaMapCoverage);
    }
    
    std::cout << std::endl << "Writing overview map: " << outputFileName << std::endl;
    overviewMap.WriteMPFile(outputFileName);
    std::cout << "Generated overview map: " << outputFileName << std::endl;
    
#ifdef DEBUG
    std::cout << std::endl << "DEBUG Build, press ENTER to continue" << std::endl;
    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
#endif
    return 0;
}



