#include "Utils.hpp"
#include "Diagnostics.hpp"

namespace Nemrod {

    /**
     * Helper method to read polish files. SectionName in callbacks are always lowercased, but lineRead is always passed as is
     * 
     * @param fileStream FileStream on the polish file
     * @param currentSectionParam If calling method with the fileStream positionned in a Section, pass-in the section name
     * @param reachedSectionStart function, first parameter is the section name
     * @param reachedSectionEnd function, first parameter is the section name
     * @param readLineInSection function, first parameter is the section name, second parameter is the line
     * @param shouldContinue function, if returns it false reading the file stops
     */
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
            
            
            if (line[0] == '[' && (line.length() > 2 && line[line.length() - 1] == ']')) {
                lineSectionName = line.substr(1, line.length() - 2);
                std::transform(lineSectionName.begin(), lineSectionName.end(), lineSectionName.begin(), ::tolower);

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
    
    bool string_equals(const std::string& str1, const std::string& str2, const bool caseSensitive) {
        if (str1.length() != str2.length()) 
            return false;        
        for (std::string::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2) {
            if (caseSensitive) {
                if (*c1 != *c2)
                    return false;
            }
            else {
                if (tolower(*c1) != tolower(*c2))
                    return false;
            }
        }
        return true;
    }
    
    template <typename T> 
    void get_shapes_max_extents(const std::vector<T>& vector, float& minLat, float& maxLat, float& minLong, float& maxLong){
        for(auto &it : vector) {
            const Shape* currentShape;
             if((currentShape = dynamic_cast<const Shape*>(&it)) != 0) {
                 for(auto &itShapePoints : currentShape->GetPoints()) {
                    for(auto &itShapePointsOnLevel : itShapePoints.second) {
                        if(itShapePointsOnLevel.GetLongitude() > maxLong)
                            maxLong = itShapePointsOnLevel.GetLongitude();                        
                        if(itShapePointsOnLevel.GetLongitude() < minLong)
                            minLong = itShapePointsOnLevel.GetLongitude();
                        if(itShapePointsOnLevel.GetLatitude() > maxLat)
                            maxLat = itShapePointsOnLevel.GetLatitude();
                        if(itShapePointsOnLevel.GetLatitude() < minLat)
                            minLat = itShapePointsOnLevel.GetLatitude();
                    }
                }
             }else{
                 std::cout << "Vector contains instances that are not of type Shape" << std::endl;
             }
        }
    }
    template void get_shapes_max_extents<Nemrod::Polyline>(const std::vector<Nemrod::Polyline>&, float&, float&, float&, float&);
    template void get_shapes_max_extents<Nemrod::Polygon>(const std::vector<Nemrod::Polygon>&, float&, float&, float&, float&);
}
