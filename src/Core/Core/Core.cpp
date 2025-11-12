#include <Core/Core.hpp>

#include <QUrl>

#include <TelemetryLoader/TelemetryLoader.hpp>

void Core::loadTelemetry(const QUrl& fileQUrl)
{
    const std::string filePath = fileQUrl.toLocalFile().toStdString();
    const std::vector<TelemetryEntry> telemetry = TelemetryLoader::LoadTelemetry(filePath);

    QVariantList telemetryList;
    telemetryList.reserve(static_cast<qsizetype>(telemetry.size()));

    const auto t0 = telemetry.front().timestamp; // ноль по времени
    for (const auto& e : telemetry) {
        const double t_sec = static_cast<double>(e.timestamp - t0) / 1000.0; // перевод после вычитания
        QVariantMap p;
        p["x"] = t_sec;
        p["y"] = e.value;
        telemetryList.push_back(p);
    }

    emit telemetryLoaded(telemetryList);
}
