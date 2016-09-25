#include "StringFormat.h"

// STL includes
#include <cstdarg>

namespace Baroque
{
    char* RawStringFormat(const char* format, ...)
    {
        va_list argList;
        va_start(argList, format);

        size_t finalSize = vsnprintf(nullptr, 0, format, argList);
        finalSize += 1;

        char* result = new char[finalSize];

        vsnprintf(const_cast<char*>(result), finalSize, format, argList);

        va_end(argList);

        return result;
    }

    std::string StringFormat(const char* format, ...)
    {
        va_list argList;
        va_start(argList, format);

        size_t finalSize = vsnprintf(nullptr, 0, format, argList);
        finalSize += 1;

        std::string result(finalSize, '\0');

        vsnprintf(const_cast<char*>(result.data()), finalSize, format, argList);

        va_end(argList);

        return result;
    }
}
