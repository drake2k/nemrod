#include "ProjectFile.hpp"
#include "../Diagnostics.h"

using namespace Nemrod::MapTk;

ProjectFile ProjectFile::LoadProjectFile(std::string fileName) {
    std::ifstream ifs(fileName);
    if(!ifs.is_open())
        EXIT_WITH_MSG("Project file does not exists or failed to open");
    
    ProjectFile projectFile;
    
    //todo trim lines, ignore case compare
    std::string line, currentSection = "END", lineSectionName;
    bool projectSectionRead = false, imgSectionRead = false;
    while ( std::getline(ifs, line) ) {
        if (line.empty() || line[0] == '#')
            continue;

        if(line[0] == '[' && (line.length() > 2 && line[line.length()-1] == ']')) {
            lineSectionName  = line.substr(1, line.length()-2);

            if(currentSection == "END")
                if(lineSectionName != "END")
                    currentSection = lineSectionName;
                else
                    EXIT_WITH_MSG("ProjectFile parsing error, overlapping END tags or END tag encountered first");
            else
                if(lineSectionName != "END")
                    EXIT_WITH_MSG("ProjectFile parsing error, overlapping sections: " + lineSectionName + " within " + currentSection);
                else {
                    if(currentSection == "Project")
                        projectSectionRead = true;
                    else if (currentSection == "IMG")
                        imgSectionRead = true;
                    currentSection = "END";
                }
            continue;
        }
                
        if(currentSection == "Project") {
            if(line.compare(0, 8,"Product=") == 0)
                projectFile._product = line.substr(8,line.length() - 8);
             else if(line.compare(0, 10, "Copyright=") == 0)            
                projectFile._copyright = line.substr(10,line.length() - 10);
        } else if (currentSection == "IMG")
            projectFile._imgs.push_back(line);
        
        // we dont support other sections
        if(projectSectionRead && imgSectionRead)
            break;
    }
    return projectFile;
}