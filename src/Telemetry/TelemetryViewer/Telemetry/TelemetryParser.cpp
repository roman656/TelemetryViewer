#include "TelemetryParser.hpp"

#include <sstream>

using namespace TelemetryViewer;

namespace
{

[[nodiscard]]
constexpr auto Trim(std::string_view line) noexcept -> std::string_view
{
    constexpr std::string_view whitespace = " \t\r\n";
    const std::size_t first = line.find_first_not_of(whitespace);

    if (first == std::string_view::npos)
    {
        return {};
    }

    const std::size_t last = line.find_last_not_of(whitespace);

    return line.substr(first, last - first + 1);    // В данном случае не бросит std::out_of_range
}



[[nodiscard]]
TelemetryMetadata ParseTelemetryMetadata(const std::string& trimmedLine)
{
    TelemetryMetadata metadata;
    std::istringstream stream { trimmedLine };

    stream >> metadata.index >> metadata.unit;
    stream >> std::ws;
    std::getline(stream, metadata.name);

    return metadata;
}



[[nodiscard]]
TelemetryEntry ParseTelemetryEntry(const std::string& trimmedLine)
{
    TelemetryEntry entry;
    std::istringstream stream { trimmedLine };
    std::string date;
    std::string time;

    stream >> date >> time >> entry.timestamp >> entry.valueCode >> entry.value;
    entry.timeString = date + " " + time;

    return entry;
}

}



Telemetry TelemetryViewer::ParseTelemetry(std::istream& input)
{
    Telemetry telemetry;
    std::string line;
    bool wasMetadataParsed = false;

    while (std::getline(input, line))
    {
        const auto trimmedLine = std::string { Trim(line) };

        if (trimmedLine.empty())
        {
            continue;
        }

        if (!wasMetadataParsed)
        {
            telemetry.metadata = ParseTelemetryMetadata(trimmedLine);
            wasMetadataParsed = true;

            continue;
        }

        telemetry.entries.push_back(ParseTelemetryEntry(trimmedLine));
    }

    return telemetry;
}
