#include "cybercba/core/Logger.hpp"

#include <cstdio>

namespace cybercba::core
{

namespace
{

const char* levelLabel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warning:
        return "WARNING";
    case LogLevel::Error:
        return "ERROR";
    }
    return "UNKNOWN";
}

} // namespace

void log(LogLevel level, std::string_view message)
{
    std::fprintf(stderr, "[%s] %.*s\n", levelLabel(level), static_cast<int>(message.size()), message.data());
}

} // namespace cybercba::core
