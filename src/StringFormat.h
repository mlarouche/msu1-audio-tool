#pragma once

// STL includes
#include <string>

namespace Baroque
{
    char* RawStringFormat(const char* format, ...);

    std::string StringFormat(const char* foramt, ...);
}
