#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <limits>

#include "mpfile/MpFile.hpp"
#include "mapTk/ProjectFile.hpp"
#include "Diagnostics.hpp"
#include "Utils.hpp"
#include "Geodata.hpp"

using namespace Nemrod;
using namespace Nemrod::MapTk;

void usage() {
    std::cout << "Usage: " << std::endl;
    // todo
    exit(1);
}

int main(int argc, char** argv) {
    // getcwd 255 magic number might be unsafe, but PATH_MAX is undefined when there is no limits
    TRACE("Current working directory(getcwd): " << getcwd(NULL, 255))
    
    std::string outputFileName, projectFileDir;

    if ((argc <= 1) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-')) 
        EXIT_WITH_MSG("No target file name was given, command must end with target file name for the overview map. Use -h for help.");
    else 
        outputFileName = argv[argc-1];
    
    opterr = 0; // suppress getopt errors
    
    std::string projectFileArg;
    bool generateAreaOfMapCoveragePoly = false;
    char opt;
    while((opt = getopt(argc -1, argv, "cp:")) != -1) {
        if(optind >= argc)
            break;
        switch(opt){
            case 'p':
                projectFileArg = optarg;
                break;
            case 'c':
                generateAreaOfMapCoveragePoly = true;
                break;
            case '?':
                usage();
                break;
        }
    }
    
    std::cout << std::endl << "Generating overview map: " << outputFileName << std::endl << std::endl;
    
    if(projectFileArg.empty())
        usage();
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
    // overview should have 2 levels
    overviewMap.GetHeader().SetLevels(2); // this should be removed and read/write from the size of the levelbits set
    
    // most detailed level needs to be lower than lowest details across maps, todo validate
    overviewMap.GetHeader().AddLevelBits(0,18);
    overviewMap.GetHeader().AddLevelBits(1,16);
    // end of header

    // body of the map
    float globalMinLat = std::numeric_limits<float>::max(),
          globalMaxLat = std::numeric_limits<float>::lowest(), 
          globalMinLong = std::numeric_limits<float>::max(), 
          globalMaxLong = std::numeric_limits<float>::lowest();
    
    for(auto &it : projectFile.GetImgs()) {
        std::cout << std::endl << "Loading: " << it << std::endl;        
        MpFile mpFile = MpFile::LoadMPFile(projectFileDir + ((projectFileDir != "")?"\\":"") + it + ".mp", false);
        std::cout << it << " loaded." << std::endl;
        
        if(mpFile.GetHeader().IsPreview()) {
            std::cout << it << " is an overview map, skipping." << std::endl;
            continue;
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
        
        // these 4 points are at boundaries of objects, we want to extend it a little bit (~200M default?)
        // todo warn or adjust depending on zoom levels, there is no data for mapsource level/precision, but
        // device precision seems to correlate
        // todo make it parameterable via a commandlinearg
        move_point_in_direction(Nemrod::NORTH_WEST, 250, topLeft);
        move_point_in_direction(Nemrod::NORTH_EAST, 250, topRight);
        move_point_in_direction(Nemrod::SOUTH_WEST, 250, botLeft);
        move_point_in_direction(Nemrod::SOUTH_EAST, 250, botRight);
        
        // create the area of map selection polygon and add to overview map
        Polygon areaMapSelection;
        areaMapSelection.SetTypeCode(0x4a);                               // ~[0x1d] means that what follows is an abbreviation
        areaMapSelection.SetLabel(mpFile.GetHeader().GetName() + " - " + mpFile.GetHeader().GetId() + "~[0x1d]" + mpFile.GetHeader().GetId());
        areaMapSelection.AddPoints(0, {topLeft, topRight, botRight, botLeft});
        overviewMap.GetPolygons().push_back(areaMapSelection);
    }
    
    if(generateAreaOfMapCoveragePoly) {
        Point topLeft(globalMaxLat, globalMinLong),
              botLeft(globalMinLat, globalMinLong),
              topRight(globalMaxLat, globalMaxLong),
              botRight(globalMinLat, globalMaxLong);
        
        // extend 400M default, 200M more than the tiles extent
        // todo make parameter
        // todo warn or adjust depending on zoom levels, there is no data for mapsource level/precision, but
        // device precision seems to correlate
        move_point_in_direction(Nemrod::NORTH_WEST, 500, topLeft);
        move_point_in_direction(Nemrod::NORTH_EAST, 500, topRight);
        move_point_in_direction(Nemrod::SOUTH_WEST, 500, botLeft);
        move_point_in_direction(Nemrod::SOUTH_EAST, 500, botRight);
        
        // create the  Area of coverage polygon and add to overview map
        Polygon areaMapCoverage;
        areaMapCoverage.SetTypeCode(0x4b);
        areaMapCoverage.SetLabel("Nemrod");
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



