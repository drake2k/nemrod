#ifndef DIAGNOSTICS_H
#define	DIAGNOSTICS_H

#include <string>
#include <iostream>
#include <cstdlib>

void EXIT_WITH_MSG(std::string errorMsg);

#ifdef DEBUG
#define TRACE(x) do { std::cout << x << std::endl; } while (0);
#else
#define TRACE(x) ;
#endif

#endif	/* DIAGNOSTICS_H */

