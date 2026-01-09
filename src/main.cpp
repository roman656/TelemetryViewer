#include <filesystem>
#include <fstream>

#include <QApplication>
#include <QCommandLineParser>

#include <TelemetryViewer/Telemetry/TelemetryParser.hpp>
#include <TelemetryViewer/TelemetryPlotting/TelemetryPlotter.hpp>

using namespace TelemetryViewer;

namespace
{

[[nodiscard]]
std::filesystem::path ParseTelemetryPathFromCliOrExit(const QApplication& application);

}



int main(int argc, char* argv[])
{
    QApplication application { argc, argv };

    QApplication::setApplicationName(QStringLiteral("TelemetryViewer"));
    QApplication::setApplicationVersion(QStringLiteral(TELEMETRY_VIEWER_VERSION));

    const std::filesystem::path telemetryFilePath = ParseTelemetryPathFromCliOrExit(application);
    std::ifstream telemetryFile { telemetryFilePath };

    if (!telemetryFile.is_open())
    {
        qCritical().noquote() << QStringLiteral("[main]: failed to open file \"%1\". Exiting...")
                .arg(QString::fromStdString(telemetryFilePath.string()));

        return 2;
    }

    const Telemetry telemetry = ParseTelemetry(telemetryFile);

    PlotTelemetry(telemetry);

    return QApplication::exec();
}



namespace
{

std::filesystem::path ParseTelemetryPathFromCliOrExit(const QApplication& application)
{
    QCommandLineParser cliParser;

    cliParser.setApplicationDescription(
            QStringLiteral("TelemetryViewer is a GUI tool for plotting parameter values from telemetry files."));
    cliParser.addHelpOption();
    cliParser.addVersionOption();
    cliParser.addPositionalArgument(QStringLiteral("telemetry_file"), QStringLiteral("Path to telemetry file."));

    cliParser.process(application);

    const QStringList positionalArgs = cliParser.positionalArguments();

    if (positionalArgs.isEmpty())
    {
        qCritical().noquote() << QStringLiteral("Error: path to telemetry file is required.\n");
        cliParser.showHelp(1);
    }

    const std::filesystem::path telemetryFilePath = positionalArgs.front().toStdString();

    return telemetryFilePath;
}

}
