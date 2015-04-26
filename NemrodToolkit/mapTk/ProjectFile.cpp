#include "ProjectFile.hpp"
#include "../Diagnostics.hpp"
#include "../Utils.hpp"

using namespace Nemrod::MapTk;

ProjectFile ProjectFile::LoadProjectFile(std::string fileName) {
    std::ifstream ifs(fileName);
    if(!ifs.is_open())
        EXIT_WITH_MSG("Project file does not exists or failed to open");
    
    ProjectFile projectFile;
    bool projectSectionRead = false, imgSectionRead = false; 
    Nemrod::polish_file_reader(ifs, NULL,
        // reaching the start of a section
        NULL,
        // reaching the end of a section
        [&projectSectionRead,&imgSectionRead] (std::string sectionName) {
            if(sectionName == "project")
                projectSectionRead = true;
            else if(sectionName == "img")
                imgSectionRead = true;
        },
        // reading a line
        [&projectFile] (std::string sectionName, std::string lineRead) {
            if(sectionName == "project") {
                if(starts_with(lineRead, "product=", false) && lineRead.length() > 8)
                    projectFile._product = lineRead.substr(8,lineRead.length() - 8);
                 else if(starts_with(lineRead, "copyright=", false) && lineRead.length() > 10)      
                    projectFile._copyright = lineRead.substr(10,lineRead.length() - 10);
                 else if(starts_with(lineRead, "familyid=", false) && lineRead.length() > 9)
                    projectFile._familyID = to_number(lineRead.substr(9, lineRead.length() - 9));
            } else if (sectionName == "img")
                projectFile._imgs.push_back(lineRead);
        },
        // shouldContinue
        [&projectSectionRead,&imgSectionRead]() {
            return !(projectSectionRead && imgSectionRead);
        }
    );
    
    return projectFile;
}