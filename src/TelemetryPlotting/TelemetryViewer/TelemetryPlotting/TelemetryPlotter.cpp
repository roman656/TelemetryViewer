#include <TelemetryViewer/TelemetryPlotting/TelemetryPlotter.hpp>

#include <algorithm>
#include <optional>
#include <utility>

#include <qcustomplot.h>

#include <TelemetryViewer/TelemetryPlotting/Constants.hpp>

using namespace TelemetryViewer;

namespace
{

struct AxisRange final
{
    double min = 0.0;
    double max = 0.0;
};



/// \brief Формирует диапазон оси по набору значений
/// \param values Набор значений
/// \returns Диапазон оси
[[nodiscard]]
AxisRange CreateAxisRange(const QVector<double>& values)
{
    const auto [minIt, maxIt] = std::minmax_element(values.cbegin(), values.cend());
    return AxisRange { *minIt, *maxIt };
}



[[nodiscard]]
QString CreateTitle(const TelemetryMetadata& metadata)
{
    if (!metadata.name.empty())
    {
        return QString::fromStdString(metadata.name);
    }

    if (!metadata.index.empty())
    {
        return QString::fromStdString(metadata.index);
    }

    return QStringLiteral("Telemetry");
}



[[nodiscard]]
QString CreateYAxisLabel(const TelemetryMetadata& metadata)
{
    QString label = QStringLiteral("value");

    if (!metadata.unit.empty())
    {
        label += QStringLiteral(" ");
        label += QString::fromStdString(metadata.unit);
    }

    return label;
}



/// \brief Добавляет поля по краям оси, чтобы график не упирался в границы
/// \param range Исходный диапазон оси
/// \returns Диапазон с добавленными полями
AxisRange AddAxisMargins(AxisRange range)
{
    if (range.min == range.max)
    {
        range.min -= 1.0;
        range.max += 1.0;
        return range;
    }

    const double margin = (range.max - range.min) * Constants::AxisMarginRatio;
    range.min -= margin;
    range.max += margin;
    return range;
}



/// \brief Формирует серии для графика
/// \param telemetry Исходные данные телеметрии
/// \returns Пары значений по осям X/Y либо std::nullopt, если данных нет
[[nodiscard]]
std::optional<std::pair<QVector<double>, QVector<double>>> CreateSeriesData(const Telemetry& telemetry)
{
    const std::size_t sampleCount = telemetry.entries.size();
    if (sampleCount == 0)
    {
        return std::nullopt;
    }

    QDateTime baseTime;
    std::uint64_t baseTimestamp = 0;
    bool baseTimeValid = false;

    QVector<double> xValues;
    QVector<double> yValues;
    xValues.resize(static_cast<int>(sampleCount));
    yValues.resize(static_cast<int>(sampleCount));

    std::transform(telemetry.entries.cbegin(), telemetry.entries.cend(), xValues.begin(),
            [&baseTime, &baseTimestamp, &baseTimeValid](const TelemetryEntry& entry)
            {
                QDateTime entryTime = QDateTime::fromString(QString::fromStdString(entry.timeString),
                                                            QStringLiteral("yyyy.MM.dd HH:mm:ss.zzz"));

                if (!baseTimeValid && entryTime.isValid())
                {
                    baseTime = entryTime;
                    baseTimestamp = entry.timestamp;
                    baseTimeValid = true;
                }

                if (!entryTime.isValid() && baseTimeValid)
                {
                    entryTime = baseTime.addMSecs(static_cast<qint64>(entry.timestamp - baseTimestamp));
                }

                return entryTime.isValid()
                        ? static_cast<double>(entryTime.toMSecsSinceEpoch()) / 1000.0
                        : static_cast<double>(entry.timestamp) / 1000.0;
            });

    std::transform(telemetry.entries.cbegin(), telemetry.entries.cend(), yValues.begin(),
            [](const TelemetryEntry& entry)
            {
                return entry.value;
            });

    return std::make_pair(std::move(xValues), std::move(yValues));
}

}



void TelemetryViewer::PlotTelemetry(const Telemetry& telemetry)
{
    auto* window = new QMainWindow();
    window->setAttribute(Qt::WA_DeleteOnClose);
    const QString appTitle = QCoreApplication::applicationName().isEmpty()
            ? QStringLiteral("TelemetryViewer")
            : QCoreApplication::applicationName();
    window->setWindowTitle(appTitle);

    auto* plot = new QCustomPlot(window);
    window->setCentralWidget(plot);
    window->resize(Constants::WindowWidth, Constants::WindowHeight);

    QVector<double> xValues;
    QVector<double> yValues;
    AxisRange xRange;
    AxisRange yRange;
    bool hasData = false;

    if (auto series = CreateSeriesData(telemetry))
    {
        xValues = std::move(series->first);
        yValues = std::move(series->second);
        xRange = CreateAxisRange(xValues);
        yRange = CreateAxisRange(yValues);
        hasData = true;
    }

    plot->addGraph();
    plot->graph(0)->setData(xValues, yValues);
    plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Constants::ScatterPointSize));
    plot->graph(0)->setAdaptiveSampling(true);
    plot->graph(0)->setName(telemetry.metadata.index.empty()
            ? QStringLiteral("index")
            : QString::fromStdString(telemetry.metadata.index));

    plot->xAxis->setLabel(QStringLiteral("timestamp"));
    plot->yAxis->setLabel(CreateYAxisLabel(telemetry.metadata));

    QFont axisLabelFont(QStringLiteral("Sans Serif"), Constants::AxisLabelFontPointSize);
    QFont tickLabelFont(QStringLiteral("Sans Serif"), Constants::TickLabelFontPointSize);
    QFont titleFont(QStringLiteral("Sans Serif"), Constants::TitleFontPointSize, QFont::Bold);

    plot->xAxis->setLabelFont(axisLabelFont);
    plot->yAxis->setLabelFont(axisLabelFont);
    plot->xAxis->setTickLabelFont(tickLabelFont);
    plot->yAxis->setTickLabelFont(tickLabelFont);
    plot->xAxis->setTickLabelRotation(Constants::AxisLabelRotationDegrees);

    auto xTicker = QSharedPointer<QCPAxisTickerDateTime>::create();
    xTicker->setDateTimeFormat(QStringLiteral("HH:mm:ss.zzz"));
    xTicker->setTickCount(Constants::AxisTickCount);
    plot->xAxis->setTicker(xTicker);

    auto yTicker = QSharedPointer<QCPAxisTicker>::create();
    yTicker->setTickCount(Constants::AxisTickCount);
    plot->yAxis->setTicker(yTicker);

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    plot->legend->setVisible(true);
    plot->legend->setFont(axisLabelFont);
    plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);

    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, CreateTitle(telemetry.metadata), titleFont));

    if (hasData)
    {
        xRange = AddAxisMargins(xRange);
        yRange = AddAxisMargins(yRange);
        plot->xAxis->setRange(xRange.min, xRange.max);
        plot->yAxis->setRange(yRange.min, yRange.max);
    }
    else
    {
        plot->xAxis->setRange(0.0, 1.0);
        plot->yAxis->setRange(0.0, 1.0);
    }

    window->show();
}
