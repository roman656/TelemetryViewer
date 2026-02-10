#ifndef TELEMETRYVIEWER_TELEMETRYPLOTTING_CONSTANTS_HPP
#define TELEMETRYVIEWER_TELEMETRYPLOTTING_CONSTANTS_HPP

namespace TelemetryViewer::Constants
{

constexpr double AxisMarginRatio = 0.03;              ///< Доля отступов от осей до самого графика
constexpr double XAxisLabelRotationDegrees = 35.0;    ///< Поворот подписей по оси X
constexpr int AxisLabelFontPointSize = 14;            ///< Размер шрифта подписи осей
constexpr int TickLabelFontPointSize = 12;            ///< Размер шрифта подписей делений
constexpr int TitleFontPointSize = 16;                ///< Размер шрифта заголовка
constexpr int ScatterPointSize = 4;                   ///< Размер точки на графике
constexpr int AxisTickCount = 9;                      ///< Количество делений оси

constexpr int WindowWidth = 1200;                     ///< Ширина окна
constexpr int WindowHeight = 800;                     ///< Высота окна

}

#endif // TELEMETRYVIEWER_TELEMETRYPLOTTING_CONSTANTS_HPP
