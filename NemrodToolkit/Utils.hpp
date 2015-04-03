#ifndef UTILS_H
#define	UTILS_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>

namespace Nemrod {

    template <typename T>
    long to_number(const std::basic_string<T> &str) {
        std::basic_stringstream<T> stream(str);
        long res;
        return !(stream >> res) ? 0 : res;
    }
    
    inline bool starts_with(std::string& toMatch, std::string prefix) {
        return toMatch.compare(0, prefix.length(), prefix) == 0;
    }   
    
    inline std::string& ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    inline std::string& rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    inline std::string& trim(std::string &s) {
        return ltrim(rtrim(s));
    }
    
    /**
     * Helper to read polish files
     * 
     * @param fileStream
     * @param currentSectionParam
     * @param reachedSectionEnd function, first parameter is the section name
     * @param readLineInSection function, first parameter is the section name, second parameter is the line
     * @param shouldContinue function, if returns it false reading the file stops
     */
    void polish_file_reader(std::ifstream& fileStream, const char* currentSectionParam,
                                std::function<void(std::string)> reachedSectionEnd,
                                std::function<void(std::string, std::string)> readLineInSection,
                                std::function<bool()> shouldContinue) ;
}

#endif	/* UTILS_H */

