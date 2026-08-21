#ifndef CYBERCBA_CORE_LOGGER_H
#define CYBERCBA_CORE_LOGGER_H

#include <cstdint>
#include <string_view>

namespace cybercba::core
{

/// Minimal logging layer for gameplay code.
///
/// The platform may redirect output; core does not depend on the mechanism.
enum class LogLevel : std::uint8_t
{
    Debug,
    Info,
    Warning,
    Error
};

/// Emits `message` at the supplied severity.
void log(LogLevel level, std::string_view message);

} // namespace cybercba::core

#endif // CYBERCBA_CORE_LOGGER_H
