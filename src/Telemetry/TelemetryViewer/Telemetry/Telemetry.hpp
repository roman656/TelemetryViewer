#ifndef TELEMETRYVIEWER_TELEMETRY_TELEMETRY_HPP
#define TELEMETRYVIEWER_TELEMETRY_TELEMETRY_HPP

#include <vector>

#include <TelemetryViewer/Telemetry/TelemetryEntry.hpp>
#include <TelemetryViewer/Telemetry/TelemetryMetadata.hpp>

namespace TelemetryViewer
{

/// \brief Телеметрия некоторого параметра
struct Telemetry final
{
    TelemetryMetadata metadata;             ///< Метаданные параметра
    std::vector<TelemetryEntry> entries;    ///< Набор измерений параметра
};

}

#endif // TELEMETRYVIEWER_TELEMETRY_TELEMETRY_HPP
