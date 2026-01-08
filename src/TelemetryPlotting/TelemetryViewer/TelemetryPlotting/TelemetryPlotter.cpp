#include <TelemetryViewer/TelemetryPlotting/TelemetryPlotter.hpp>

#include <algorithm>
#include <cstdint>
#include <limits>

#include <QApplication>
#include <QBrush>
#include <QColor>
#include <QDateTime>
#include <QFont>
#include <QMainWindow>
#include <QPen>
#include <QSharedPointer>
#include <QVector>
#include <qcustomplot.h>

using namespace TelemetryViewer;

namespace
{

[[nodiscard]]
QFont IncreaseFontSize(QFont font, int pointDelta, int pixelDelta)
{
    if (font.pointSize() > 0)
    {
        font.setPointSize(font.pointSize() + pointDelta);
    }
    else if (font.pixelSize() > 0)
    {
        font.setPixelSize(font.pixelSize() + pixelDelta);
    }
    return font;
}



[[nodiscard]]
QString BuildTitle(const TelemetryMetadata& metadata)
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
QString BuildYAxisLabel(const TelemetryMetadata& metadata)
{
    QString label = QStringLiteral("value");

    if (!metadata.unit.empty())
    {
        label += QStringLiteral(" ");
        label += QString::fromStdString(metadata.unit);
    }

    return label;
}



void ExpandRange(double& minValue, double& maxValue)
{
    if (minValue == maxValue)
    {
        minValue -= 1.0;
        maxValue += 1.0;
        return;
    }

    const double margin = (maxValue - minValue) * 0.05;
    minValue -= margin;
    maxValue += margin;
}



bool FillSeriesData(const Telemetry& telemetry, QVector<double>& xValues, QVector<double>& yValues,
                    double& minX, double& maxX, double& minY, double& maxY)
{
    const std::size_t sampleCount = telemetry.entries.size();
    if (sampleCount == 0)
    {
        return false;
    }

    xValues.reserve(static_cast<int>(sampleCount));
    yValues.reserve(static_cast<int>(sampleCount));

    QDateTime baseTime;
    std::uint64_t baseTimestamp = 0;
    bool baseTimeValid = false;

    for (const auto& entry : telemetry.entries)
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

        const double x = entryTime.isValid()
                ? static_cast<double>(entryTime.toMSecsSinceEpoch()) / 1000.0
                : static_cast<double>(entry.timestamp) / 1000.0;
        const double y = entry.value;

        xValues.push_back(x);
        yValues.push_back(y);

        minX = std::min(minX, x);
        maxX = std::max(maxX, x);
        minY = std::min(minY, y);
        maxY = std::max(maxY, y);
    }

    return true;
}

}


void TelemetryViewer::PlotTelemetry(const Telemetry& telemetry)
{
    int argc = 1;
    char appName[] = "TelemetryViewer";
    char* argv[] = { appName, nullptr };
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle(QStringLiteral("TelemetryViewer"));

    auto* plot = new QCustomPlot(&window);
    window.setCentralWidget(plot);
    window.resize(1200, 700);

    QVector<double> xValues;
    QVector<double> yValues;
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();
    const bool hasData = FillSeriesData(telemetry, xValues, yValues, minX, maxX, minY, maxY);

    plot->addGraph();
    plot->graph(0)->setData(xValues, yValues);
    plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    plot->graph(0)->setAdaptiveSampling(true);
    plot->graph(0)->setName(telemetry.metadata.index.empty()
            ? QStringLiteral("index")
            : QString::fromStdString(telemetry.metadata.index));

    plot->xAxis->setLabel(QStringLiteral("timestamp"));
    plot->yAxis->setLabel(BuildYAxisLabel(telemetry.metadata));

    QFont axisLabelFont = IncreaseFontSize(plot->xAxis->labelFont(), 3, 6);
    QFont tickLabelFont = IncreaseFontSize(plot->xAxis->tickLabelFont(), 2, 4);
    QFont titleFont(QStringLiteral("Sans Serif"), 15, QFont::Bold);

    plot->xAxis->setLabelFont(axisLabelFont);
    plot->yAxis->setLabelFont(axisLabelFont);
    plot->xAxis->setTickLabelFont(tickLabelFont);
    plot->yAxis->setTickLabelFont(tickLabelFont);
    plot->xAxis->setTickLabelRotation(45.0);

    auto xTicker = QSharedPointer<QCPAxisTickerDateTime>(new QCPAxisTickerDateTime);
    xTicker->setDateTimeFormat(QStringLiteral("HH:mm:ss.zzz"));
    xTicker->setTickCount(12);
    plot->xAxis->setTicker(xTicker);

    auto yTicker = QSharedPointer<QCPAxisTicker>(new QCPAxisTicker);
    yTicker->setTickCount(12);
    plot->yAxis->setTicker(yTicker);

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    plot->legend->setVisible(true);
    plot->legend->setBorderPen(QPen(QColor(30, 30, 30, 200), 1));
    plot->legend->setBrush(QBrush(QColor(255, 255, 255, 240)));
    plot->legend->setFont(axisLabelFont);
    plot->legend->setIconSize(26, 12);
    plot->legend->setIconTextPadding(8);
    plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);

    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, BuildTitle(telemetry.metadata), titleFont));

    if (hasData)
    {
        ExpandRange(minX, maxX);
        ExpandRange(minY, maxY);
        plot->xAxis->setRange(minX, maxX);
        plot->yAxis->setRange(minY, maxY);
    }
    else
    {
        plot->xAxis->setRange(0.0, 1.0);
        plot->yAxis->setRange(0.0, 1.0);
    }

    window.show();
    app.exec();
}
