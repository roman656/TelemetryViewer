#include <TelemetryLoader/TelemetryLoader.hpp>

#include <fstream>

std::vector<TelemetryEntry> TelemetryLoader::LoadTelemetry(const std::filesystem::path& filename)
{
    std::vector<TelemetryEntry> telemetryEntries;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        //throw std::runtime_error("[TelemetryLoader::LoadTelemetry]: could not open file");
    }

    telemetryEntries.push_back(TelemetryEntry(1, 0.5));
    telemetryEntries.push_back(TelemetryEntry(2, 0.6));
    telemetryEntries.push_back(TelemetryEntry(3, 0.9));
    telemetryEntries.push_back(TelemetryEntry(4, 0.2));
    telemetryEntries.push_back(TelemetryEntry(5, 0.0));

    return telemetryEntries;
}
