#include "Utils.hpp"
#include "Diagnostics.hpp"

namespace Nemrod {

    void polish_file_reader(std::ifstream& fileStream, const char* currentSectionParam,
            std::function<void(std::string) > reachedSectionStart,
            std::function<void(std::string) > reachedSectionEnd,
            std::function<void(std::string, std::string) > readLineInSection,
            std::function<bool() > shouldContinue) {

        std::string line, currentSection = currentSectionParam != NULL ? currentSectionParam : "end", lineSectionName;
        while (std::getline(fileStream, line)) {
            if (line.empty() || line[0] == '#' || line[0] == ';')
                continue;
            
            trim(line);
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);
            
            if (line[0] == '[' && (line.length() > 2 && line[line.length() - 1] == ']')) {
                lineSectionName = line.substr(1, line.length() - 2);

                // todo replace starts_with logic with sectionName=="end" or section=="end-sectionName" (Spec. 4.2))
                if (starts_with(currentSection, "end"))
                    if (!starts_with(lineSectionName, "end")) {
                        if(reachedSectionStart)
                            reachedSectionStart(lineSectionName);
                        currentSection = lineSectionName;
                    }
                    else
                        EXIT_WITH_MSG("ProjectFile parsing error, overlapping END tags or END tag encountered first");
                else {
                    if (!starts_with(lineSectionName, "end"))
                        EXIT_WITH_MSG("ProjectFile parsing error, overlapping sections: " + lineSectionName + " within " + currentSection);
                    else {
                        if(reachedSectionEnd)
                            reachedSectionEnd(currentSection);
                        currentSection = "end";
                    }
                }
            }else{
                readLineInSection(currentSection, line);
            }
            // we dont support other sections
            if (shouldContinue && !shouldContinue())
                break;
        }
    }
}
