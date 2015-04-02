#ifndef UTILS_H
#define	UTILS_H

#include <string>
#include <sstream>

namespace Nemrod {

    template <typename T>
    long to_number(const std::basic_string<T> &str) {
        std::basic_stringstream<T> stream(str);
        long res;
        return !(stream >> res) ? 0 : res;
    }
    
    bool starts_with(std::string& toMatch, std::string prefix) ;
}

#endif	/* UTILS_H */

