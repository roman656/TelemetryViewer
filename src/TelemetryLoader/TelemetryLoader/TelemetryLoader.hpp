#ifndef TV_TELEMETRYLOADER_TELEMETRYLOADER_HPP
#define TV_TELEMETRYLOADER_TELEMETRYLOADER_HPP

#include <filesystem>
#include <vector>

#include <TelemetryLoader/TelemetryEntry.hpp>

namespace TelemetryLoader
{

std::vector<TelemetryEntry> LoadTelemetry(const std::filesystem::path& filename);

};

#endif // TV_TELEMETRYLOADER_TELEMETRYLOADER_HPP
