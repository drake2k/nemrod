#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <limits>

#include "mpfile/MpFile.hpp"
#include "mapTk/ProjectFile.hpp"
#include "Diagnostics.hpp"

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
    
    for(auto &it : projectFile.GetImgs()) {
        std::cout << std::endl << "Loading: " << it << std::endl;        
        MpFile mpFile = MpFile::LoadMPFile(workingDir + "\\" + it + ".mp", false);
        std::cout << it << " loaded." << std::endl;
        
        if(mpFile.GetHeader().IsPreview()) {
            std::cout << it << " is an overview map, skipping." << std::endl;
            continue;
        }
        
        
    }
    std::cout << std::endl << "Generated overview map: " << outputFileName << std::endl;
    
#ifdef DEBUG
    std::cout << "DEBUG Build, press ENTER to continue" << std::endl;
    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
#endif
    return 0;
}

