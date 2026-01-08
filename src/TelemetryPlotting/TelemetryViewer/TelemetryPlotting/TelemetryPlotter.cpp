#include <TelemetryViewer/TelemetryPlotting/TelemetryPlotter.hpp>

#include <sciplot/sciplot.hpp>

using namespace sciplot;
using namespace TelemetryViewer;

void TelemetryViewer::PlotTelemetry(const Telemetry& telemetry)
{
    const std::size_t entriesAmount = telemetry.entries.size();
    Vec values(entriesAmount);
    Vec timestamps(entriesAmount);

    // double: value
    // std::uint64_t: timestamp

    std::transform(
            telemetry.entries.cbegin(), telemetry.entries.cend(),
            begin(values),
            [](const TelemetryEntry& entry)
            {
                return entry.value;
            });

    const std::uint64_t startTimestamp = telemetry.entries.front().timestamp;

    std::transform(
            telemetry.entries.cbegin(), telemetry.entries.cend(),
            begin(timestamps),
            [startTimestamp](const TelemetryEntry& entry)
            {
                return static_cast<double>(startTimestamp - entry.timestamp);
            });

    Plot2D plot;
    plot.fontName("Palatino");
    plot.drawCurveWithPoints(timestamps, values).label("tm");
    plot.xlabel("timestamp");
    plot.ylabel("value");
    plot.grid().show();

    Figure figure {{ plot }};
    figure.palette("dark2");

    Canvas canvas {{ figure }};
    canvas.size(1200, 800);
    canvas.fontSize(16);
    canvas.title("Telemetry Viewer");

    canvas.show();
}
