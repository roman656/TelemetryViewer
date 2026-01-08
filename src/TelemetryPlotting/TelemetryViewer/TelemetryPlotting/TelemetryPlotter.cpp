#include <TelemetryViewer/TelemetryPlotting/TelemetryPlotter.hpp>

#include <algorithm>
#include <limits>
#include <memory>

#include <QApplication>
#include <QBrush>
#include <QColor>
#include <QCoreApplication>
#include <QFont>
#include <QMainWindow>
#include <QPen>
#include <QSharedPointer>
#include <QVector>
#include <qcustomplot.h>

using namespace TelemetryViewer;

namespace
{

struct PlotRanges final
{
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();
    bool hasData = false;
};



struct PlotFonts final
{
    QFont axisLabel;
    QFont tickLabel;
    QFont title;
};



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
std::unique_ptr<QApplication> CreateApplicationIfNeeded()
{
    if (QCoreApplication::instance() != nullptr)
    {
        return nullptr;
    }

    static int argc = 1;
    static char appName[] = "TelemetryViewer";
    static char* argv[] = { appName, nullptr };

    return std::make_unique<QApplication>(argc, argv);
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



void UpdateRange(double value, double& minValue, double& maxValue)
{
    minValue = std::min(minValue, value);
    maxValue = std::max(maxValue, value);
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



[[nodiscard]]
PlotFonts BuildFonts(QCustomPlot* plot)
{
    PlotFonts fonts;
    fonts.axisLabel = IncreaseFontSize(plot->xAxis->labelFont(), 3, 6);
    fonts.tickLabel = IncreaseFontSize(plot->xAxis->tickLabelFont(), 2, 4);
    fonts.title = QFont(QStringLiteral("Sans Serif"), 15, QFont::Bold);

    return fonts;
}



[[nodiscard]]
PlotRanges BuildSeriesData(const Telemetry& telemetry, QVector<double>& xValues, QVector<double>& yValues)
{
    PlotRanges ranges;
    const std::size_t sampleCount = telemetry.entries.size();

    xValues.reserve(static_cast<int>(sampleCount));
    yValues.reserve(static_cast<int>(sampleCount));

    for (const auto& entry : telemetry.entries)
    {
        const double x = static_cast<double>(entry.timestamp);
        const double y = entry.value;

        xValues.push_back(x);
        yValues.push_back(y);

        UpdateRange(x, ranges.minX, ranges.maxX);
        UpdateRange(y, ranges.minY, ranges.maxY);
    }

    ranges.hasData = (sampleCount > 0);
    return ranges;
}



void ConfigureGraph(QCustomPlot* plot, const TelemetryMetadata& metadata,
                    const QVector<double>& xValues, const QVector<double>& yValues)
{
    plot->addGraph();
    plot->graph(0)->setData(xValues, yValues);
    plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    plot->graph(0)->setAdaptiveSampling(true);
    plot->graph(0)->setName(metadata.index.empty()
            ? QStringLiteral("index")
            : QString::fromStdString(metadata.index));
}



void ConfigureAxes(QCustomPlot* plot, const TelemetryMetadata& metadata, const PlotFonts& fonts)
{
    plot->xAxis->setLabel(QStringLiteral("timestamp [ms]"));
    plot->yAxis->setLabel(BuildYAxisLabel(metadata));

    plot->xAxis->setLabelFont(fonts.axisLabel);
    plot->yAxis->setLabelFont(fonts.axisLabel);
    plot->xAxis->setTickLabelFont(fonts.tickLabel);
    plot->yAxis->setTickLabelFont(fonts.tickLabel);
    plot->xAxis->setTickLabelRotation(45.0);

    auto xTicker = QSharedPointer<QCPAxisTicker>(new QCPAxisTicker);
    xTicker->setTickCount(12);
    plot->xAxis->setTicker(xTicker);

    auto yTicker = QSharedPointer<QCPAxisTicker>(new QCPAxisTicker);
    yTicker->setTickCount(12);
    plot->yAxis->setTicker(yTicker);
}



void ConfigureLegend(QCustomPlot* plot, const QFont& font)
{
    if (!plot->legend)
    {
        return;
    }

    plot->legend->setVisible(true);
    plot->legend->setBorderPen(QPen(QColor(30, 30, 30, 200), 1));
    plot->legend->setBrush(QBrush(QColor(255, 255, 255, 240)));
    plot->legend->setFont(font);
    plot->legend->setIconSize(26, 12);
    plot->legend->setIconTextPadding(8);
    plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
}



void ConfigureTitle(QCustomPlot* plot, const TelemetryMetadata& metadata, const QFont& font)
{
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, BuildTitle(metadata), font));
}



void ConfigureInteractions(QCustomPlot* plot)
{
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
}



void ApplyRanges(QCustomPlot* plot, const PlotRanges& ranges)
{
    if (ranges.hasData)
    {
        double minX = ranges.minX;
        double maxX = ranges.maxX;
        double minY = ranges.minY;
        double maxY = ranges.maxY;

        ExpandRange(minX, maxX);
        ExpandRange(minY, maxY);
        plot->xAxis->setRange(minX, maxX);
        plot->yAxis->setRange(minY, maxY);
        return;
    }

    plot->xAxis->setRange(0.0, 1.0);
    plot->yAxis->setRange(0.0, 1.0);
}

}


void TelemetryViewer::PlotTelemetry(const Telemetry& telemetry)
{
    std::unique_ptr<QApplication> ownedApp = CreateApplicationIfNeeded();

    auto* window = new QMainWindow();
    window->setAttribute(Qt::WA_DeleteOnClose);
    const QString appTitle = QCoreApplication::applicationName().isEmpty()
            ? QStringLiteral("TelemetryViewer")
            : QCoreApplication::applicationName();
    window->setWindowTitle(appTitle);

    auto* plot = new QCustomPlot(window);
    QVector<double> xValues;
    QVector<double> yValues;

    const PlotFonts fonts = BuildFonts(plot);
    const PlotRanges ranges = BuildSeriesData(telemetry, xValues, yValues);

    ConfigureGraph(plot, telemetry.metadata, xValues, yValues);
    ConfigureAxes(plot, telemetry.metadata, fonts);
    ConfigureInteractions(plot);
    ConfigureLegend(plot, fonts.axisLabel);
    ConfigureTitle(plot, telemetry.metadata, fonts.title);
    ApplyRanges(plot, ranges);

    window->setCentralWidget(plot);
    window->resize(1200, 700);
    window->show();

    if (ownedApp)
    {
        ownedApp->exec();
    }
    else
    {
        plot->replot();
    }
}
