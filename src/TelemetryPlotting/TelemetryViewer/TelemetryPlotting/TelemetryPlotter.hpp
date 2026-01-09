#ifndef TELEMETRYVIEWER_TELEMETRYPLOTTING_TELEMETRYPLOTTER_HPP
#define TELEMETRYVIEWER_TELEMETRYPLOTTING_TELEMETRYPLOTTER_HPP

#include <TelemetryViewer/Telemetry/Telemetry.hpp>

namespace TelemetryViewer
{

/// \brief Функция отрисовки графика телеметрии в отдельном окне
/// \param telemetry Телеметрия, которую нужно отобразить
/// \details Требуется созданный экземпляр QApplication до вызова функции
void PlotTelemetry(const Telemetry& telemetry);

}

#endif // TELEMETRYVIEWER_TELEMETRYPLOTTING_TELEMETRYPLOTTER_HPP
