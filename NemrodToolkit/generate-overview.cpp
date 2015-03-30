#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>

#include "mpfile/MpFile.hpp"

using namespace Nemrod;
using namespace std;

void usage() {
    cout << "Usage: " << endl;
    // todo
    exit(1);
}

int main(int argc, char** argv) {
    
    string outputFileName;

    if ((argc <= 1) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-')) {
        cout << "No target file name was given, command must end with target file name for the overview map. Use -h for help." << endl;
        exit(1);
    }
    else 
        outputFileName = argv[argc-1];
    
    opterr = 0; // suppress getopt errors
    
    string projectFile;
    char opt;
    while((opt = getopt(argc -1, argv, "p:")) != -1) {
        if(optind >= argc)
            break;
        switch(opt){
            case 'p':
                projectFile = optarg;
                break;
            case '?':
                usage();
                break;
        }
    }
    
    cout << "Generating overview map" << outputFileName << endl;
    
    cout << "Generated overview map: " << outputFileName << endl;
}

