#ifndef TELEMETRYVIEWER_TELEMETRY_TELEMETRYMETADATA_HPP
#define TELEMETRYVIEWER_TELEMETRY_TELEMETRYMETADATA_HPP

#include <string>

namespace TelemetryViewer
{

/// \brief Метаданные параметра телеметрии
struct TelemetryMetadata final
{
    std::string index;    ///< Индекс параметра
    std::string unit;     ///< Размерность параметра
    std::string name;     ///< Наименование параметра
};

}
#endif // TELEMETRYVIEWER_TELEMETRY_TELEMETRYMETADATA_HPP
