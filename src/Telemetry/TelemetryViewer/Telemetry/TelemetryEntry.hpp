#ifndef TELEMETRYVIEWER_TELEMETRY_TELEMETRYENTRY_HPP
#define TELEMETRYVIEWER_TELEMETRY_TELEMETRYENTRY_HPP

#include <cstdint>
#include <string>

namespace TelemetryViewer
{

/// \brief Запись в телеметрии (значение параметра в конкретный момент времени)
struct TelemetryEntry final
{
    std::string timeString;         ///< Время регистрации значения в текстовом формате
    std::uint64_t timestamp = 0;    ///< Время регистрации значения в миллисекундах
    std::uint32_t valueCode = 0;    ///< Код значения
    double value = 0.0;             ///< Значение в размерности физической величины
};

}

#endif // TELEMETRYVIEWER_TELEMETRY_TELEMETRYENTRY_HPP
