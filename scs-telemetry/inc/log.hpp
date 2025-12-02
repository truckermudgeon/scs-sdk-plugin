#if LOGGING
#ifndef LOGGER
#define LOGGER
#include <fstream>
#include <string>

namespace logger
{
    extern const std::string path;
    extern std::ofstream out;
    void flush();
} // namespace logger

#endif
#endif