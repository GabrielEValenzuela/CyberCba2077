#ifndef CYBERCBA_CORE_LOGGER_H
#define CYBERCBA_CORE_LOGGER_H

#include <cstdint>
#include <string_view>

namespace cybercba::core
{

// Minimal logging layer (TAD-001 §48). Gameplay code logs through this, never
// through ad-hoc printf(); the raylib platform layer may redirect output but
// core must not depend on how logs are actually emitted.
enum class LogLevel : std::uint8_t
{
    Debug,
    Info,
    Warning,
    Error
};

void log(LogLevel level, std::string_view message);

} // namespace cybercba::core

#endif // CYBERCBA_CORE_LOGGER_H
