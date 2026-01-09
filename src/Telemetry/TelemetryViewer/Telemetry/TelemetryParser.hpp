#ifndef TELEMETRYVIEWER_TELEMETRY_TELEMETRYPARSER_HPP
#define TELEMETRYVIEWER_TELEMETRY_TELEMETRYPARSER_HPP

#include <TelemetryViewer/Telemetry/Telemetry.hpp>

namespace TelemetryViewer
{

/// \brief Функция парса телеметрии из текстового потока
/// \param input Входной поток с телеметрией
/// \returns Заполненную структуру телеметрии
/// \details Формат: первая непустая строка содержит index, unit и name;
/// далее идут строки вида "date time timestamp valueCode value". Пустые строки игнорируются
[[nodiscard]]
Telemetry ParseTelemetry(std::istream& input);

}

#endif // TELEMETRYVIEWER_TELEMETRY_TELEMETRYPARSER_HPP
