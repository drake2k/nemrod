#ifndef UTILS_H
#define	UTILS_H

#include <ios>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <iomanip>

namespace Nemrod {

    template <typename T>
    long to_number(const std::basic_string<T> &str) {
        std::basic_stringstream<T> stream(str);
        long res;
        return !(stream >> res) ? 0 : res;
    }
    
    template <typename T>
    long to_number_from_hex(const std::basic_string<T> &str) {
        std::basic_stringstream<T> stream(str);
        long res;
        return !(stream >> std::hex >> res) ? 0 : res;
    }
    
    template< typename T >
    std::string to_hex_from_number( T i ){
        std::stringstream stream;
        stream << std::showbase
               << std::setfill ('0') << std::setw(sizeof(T)*2) 
               << std::hex << i;
        return stream.str();
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
                                std::function<void(std::string)> reachedSectionStart,
                                std::function<void(std::string)> reachedSectionEnd,
                                std::function<void(std::string, std::string)> readLineInSection,
                                std::function<bool()> shouldContinue) ;
}

#endif	/* UTILS_H */