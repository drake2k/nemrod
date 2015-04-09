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
#include <utility>

namespace Nemrod {

    template <typename T>
    long to_number(const std::basic_string<T>& str) {
        std::basic_stringstream<T> stream(str);
        long res;
        return !(stream >> res) ? 0 : res;
    }
    
    template <typename T>
    float to_float(const std::basic_string<T>& str) {
        std::basic_stringstream<T> stream(str);
        float res;
        return !(stream >> res) ? 0 : res;
    }
    
    template <typename T>
    long to_number_from_hex(const std::basic_string<T>& str) {
        std::basic_stringstream<T> stream(str);
        long res;
        return !(stream >> std::hex >> res) ? 0 : res;
    }

    template< typename T >
    std::string to_hex_from_number(T i) {
        std::stringstream stream;
        stream << std::showbase
                << std::setfill('0') << std::setw(sizeof (T)*2)
                << std::hex << i;
        return stream.str();
    }

    bool string_equals(const std::string& str1, const std::string& str2, const bool caseSensitive);

    inline bool starts_with(const std::string& toMatch, const std::string& prefix, const bool caseSensitive) {
        if(prefix.length() > toMatch.length())
            return false;
        return string_equals(toMatch.substr(0, prefix.length()), prefix, caseSensitive);
    }
    
    inline bool starts_with(const std::string& toMatch, const std::string& prefix) {
        return starts_with(toMatch, prefix, true);
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
            std::function<bool() > shouldContinue);
    
    float read_polish_float(const std::string& attrName, const std::string& polishLine);
    int read_polish_int(const std::string& attrName, const std::string& polishLine);
    char read_polish_char(const std::string& attrName, const std::string& polishLine);
    bool read_polish_bool(const std::string& attrName, const std::string& polishLine);
    std::string read_polish_string(const std::string& attrName, const std::string& polishLine);
    
    std::pair<int,std::string> read_polish_leveledstring(const std::string& attrName, const std::string& polishLine);
    std::pair<int,int> read_polish_leveledint(const std::string& attrName, const std::string& polishLine);
    
}

#endif	/* UTILS_H */