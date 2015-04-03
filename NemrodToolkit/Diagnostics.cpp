#include "Diagnostics.hpp"
#include <cstdlib>
#include <limits>

void EXIT_WITH_MSG(std::string errorMsg) {
    std::cerr << errorMsg << std::endl;
#ifdef DEBUG
    std::cout << std::endl << std::endl << "Diagnostics forcing an exit." << std::endl;
    std::cout << "DEBUG Build, press ENTER to continue" << std::endl;
    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
#endif
    exit(1);
}