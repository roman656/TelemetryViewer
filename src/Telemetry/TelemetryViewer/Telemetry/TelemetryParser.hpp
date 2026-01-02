#ifndef TELEMETRYVIEWER_TELEMETRY_TELEMETRYPARSER_HPP
#define TELEMETRYVIEWER_TELEMETRY_TELEMETRYPARSER_HPP

#include <TelemetryViewer/Telemetry/Telemetry.hpp>

namespace TelemetryViewer
{

[[nodiscard]]
Telemetry ParseTelemetry(std::istream& input);

}

#endif // TELEMETRYVIEWER_TELEMETRY_TELEMETRYPARSER_HPP
