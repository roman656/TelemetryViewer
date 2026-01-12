#include <TelemetryViewer/TelemetryPlotting/TelemetryPlotter.hpp>

#include <qcustomplot.h>

#include <TelemetryViewer/TelemetryPlotting/Constants.hpp>

using namespace TelemetryViewer;

namespace
{

[[nodiscard]]
QString CreateParameterAxisLabel(const TelemetryMetadata& metadata)
{
    auto label = QStringLiteral("value");

    if (!metadata.unit.empty())
    {
        label += QStringLiteral(" ");
        label += QString::fromStdString(metadata.unit);
    }

    return label;
}



[[nodiscard]]
std::pair<double, double> AddAxisRangeMargins(double axisMinValue, double axisMaxValue)
{
    if (axisMinValue == axisMaxValue)
    {
        axisMinValue -= 1;
        axisMaxValue += 1;

        return { axisMinValue, axisMaxValue };
    }

    const double margin = (axisMaxValue - axisMinValue) * Constants::AxisMarginRatio;

    axisMinValue -= margin;
    axisMaxValue += margin;

    return { axisMinValue, axisMaxValue };
}



[[nodiscard]]
std::pair<double, double> CreateAxisRange(const QVector<double>& axisData)
{
    if (axisData.isEmpty())
    {
        return { 0, 1 };
    }

    const auto [minElemIt, maxElemIt] = std::minmax_element(axisData.cbegin(), axisData.cend());

    return AddAxisRangeMargins(*minElemIt, *maxElemIt);
}



[[nodiscard]]
std::pair<QVector<double>, QVector<double>> CreatePlotData(const Telemetry& telemetry)
{
    const std::size_t entriesAmount = telemetry.entries.size();

    if (entriesAmount == 0)
    {
        return {};
    }

    QVector<double> xValues;
    QVector<double> yValues;

    xValues.resize(static_cast<qsizetype>(entriesAmount));
    yValues.resize(static_cast<qsizetype>(entriesAmount));

    std::transform(
            telemetry.entries.cbegin(), telemetry.entries.cend(),
            xValues.begin(),
            [](const TelemetryEntry& entry)
            {
                constexpr double millisecondsPerSecond = 1000.0;

                return static_cast<double>(entry.timestamp) / millisecondsPerSecond;
            });

    std::transform(
            telemetry.entries.cbegin(), telemetry.entries.cend(),
            yValues.begin(),
            [](const TelemetryEntry& entry) { return entry.value; });

    return { xValues, yValues };
}

}



void TelemetryViewer::PlotTelemetry(const Telemetry& telemetry)
{
    auto* window = new QMainWindow();
    auto* plot = new QCustomPlot(window);
    auto* graph = plot->addGraph();

    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setCentralWidget(plot);
    window->resize(Constants::WindowWidth, Constants::WindowHeight);

    const auto [xValues, yValues] = CreatePlotData(telemetry);
    const auto [minXAxisValue, maxXAxisValue] = CreateAxisRange(xValues);
    const auto [minYAxisValue, maxYAxisValue] = CreateAxisRange(yValues);

    const auto fontFamily = QStringLiteral("Sans Serif");
    const QFont axisLabelFont { fontFamily, Constants::AxisLabelFontPointSize };
    const QFont tickLabelFont { fontFamily, Constants::TickLabelFontPointSize };
    const QFont titleFont { fontFamily, Constants::TitleFontPointSize, QFont::Bold };

    graph->setData(xValues, yValues);
    graph->setScatterStyle(QCPScatterStyle { QCPScatterStyle::ssCircle, Constants::ScatterPointSize });

    /* Настройка осей */
    plot->xAxis->setRange(minXAxisValue, maxXAxisValue);
    plot->yAxis->setRange(minYAxisValue, maxYAxisValue);

    plot->xAxis->setLabel(QStringLiteral("timestamp"));
    plot->yAxis->setLabel(CreateParameterAxisLabel(telemetry.metadata));

    plot->xAxis->setLabelFont(axisLabelFont);
    plot->yAxis->setLabelFont(axisLabelFont);

    plot->xAxis->setTickLabelFont(tickLabelFont);
    plot->yAxis->setTickLabelFont(tickLabelFont);

    plot->xAxis->setTickLabelRotation(Constants::AxisLabelRotationDegrees);

    const auto xTicker = QSharedPointer<QCPAxisTickerDateTime>::create();
    const auto yTicker = QSharedPointer<QCPAxisTicker>::create();

    xTicker->setDateTimeFormat(QStringLiteral("HH:mm:ss.zzz"));
    xTicker->setDateTimeSpec(Qt::UTC);

    xTicker->setTickCount(Constants::AxisTickCount);
    yTicker->setTickCount(Constants::AxisTickCount);

    plot->xAxis->setTicker(xTicker);
    plot->yAxis->setTicker(yTicker);

    /* Включение панорамирования и масштабирования */
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    /* Добавление заголовка над графиком */
    if (!telemetry.metadata.name.empty())
    {
        const auto plotTitle = QString::fromStdString(telemetry.metadata.name);

        plot->plotLayout()->insertRow(0);
        plot->plotLayout()->addElement(0, 0, new QCPTextElement { plot, plotTitle, titleFont });
    }

    /* Добавление подписи легенды */
    if (!telemetry.metadata.index.empty())
    {
        graph->setName(QString::fromStdString(telemetry.metadata.index));

        plot->legend->setVisible(true);
        plot->legend->setFont(axisLabelFont);
    }

    window->show();
}
