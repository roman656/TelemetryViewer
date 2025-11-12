#ifndef TV_TELEMETRYLOADER_TELEMETRYENTRY_HPP
#define TV_TELEMETRYLOADER_TELEMETRYENTRY_HPP

#include <cstdint>

struct TelemetryEntry final
{
    std::uint64_t timestamp = 0;
    double value = 0.0;
};

#endif // TV_TELEMETRYLOADER_TELEMETRYENTRY_HPP
