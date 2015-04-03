#include <iostream>
#include "Utils.hpp"
#include <string>

int main() {
    
    std::string test = "  lol  lolol \t";
    Nemrod::trim(test);
    std::cout << test;
    
#if DEBUG
    std::cout << "Hello, World Debug!" << std::endl;
#else
    std::cout << "Hello, World Release!" << std::endl;
#endif
}