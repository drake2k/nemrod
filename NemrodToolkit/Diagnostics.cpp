#include "Diagnostics.h"
#include <cstdlib>

void EXIT_WITH_MSG(std::string errorMsg) {
    std::cerr << errorMsg << std::endl;
    exit(1);
}