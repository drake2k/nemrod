#include <iostream>

#include "Diagnostics.hpp"
#include <string>

int main() {
    
    std::string test = "  lol  lolol \t";
    std::cout << test;
    
#if DEBUG
    std::cout << "Hello, World Debug!" << std::endl;
#else
    std::cout << "Hello, World Release!" << std::endl;
#endif
}