#include "Utils.hpp"

namespace Nemrod {
    bool starts_with(std::string& toMatch, std::string prefix) {
        return toMatch.compare(0, prefix.length(), prefix);
    }
}
