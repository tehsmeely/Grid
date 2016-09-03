#include "paths.h"


std::string pathAppend(const std::string& p1, const std::string& p2)
{

	char sep = '/';
	std::string tmp = p1;

#ifdef _WIN32
	sep = '\\';
#endif

	if (p1[p1.length()] != sep) { // Need to add a
		tmp += sep;               // path separator
		return(tmp + p2);
	}
	else
		return(p1 + p2);
}

// This basic function pinched from C++ Cookbook, D.R. Stephens, C. Diggins, J. Turkanis, J. Cogswell (O'Reilly)