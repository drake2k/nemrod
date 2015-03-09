#include <iostream>

int main() {
#if DEBUG
    std::cout << "Hello, World Debug!" << std::endl;
#else
    std::cout << "Hello, World Release!" << std::endl;
#endif
}