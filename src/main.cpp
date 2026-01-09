#include <fstream>

#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>

#include <TelemetryViewer/Telemetry/TelemetryParser.hpp>
#include <TelemetryViewer/TelemetryPlotting/TelemetryPlotter.hpp>

using namespace TelemetryViewer;

int main(int argc, char* argv[])
{
    QApplication app { argc, argv };

    QApplication::setApplicationName(QStringLiteral("TelemetryViewer"));
    QApplication::setApplicationVersion(QStringLiteral(TELEMETRY_VIEWER_VERSION));

    QCommandLineParser cliParser;

    cliParser.setApplicationDescription(
            QStringLiteral("TelemetryViewer is a GUI tool for plotting parameter values from telemetry files."));
    cliParser.addHelpOption();
    cliParser.addVersionOption();
    cliParser.addPositionalArgument(QStringLiteral("telemetry_file"), QStringLiteral("Path to telemetry file."));

    cliParser.process(app);

    const QStringList positionalArgs = cliParser.positionalArguments();

    if (positionalArgs.isEmpty())
    {
        qCritical().noquote() << QStringLiteral("Error: path to telemetry file is required.\n");
        cliParser.showHelp(1);
    }

    const QFileInfo telemetryFileInfo { positionalArgs.front() };

    if (!telemetryFileInfo.exists() || !telemetryFileInfo.isFile())
    {
        qCritical().noquote() <<
                QStringLiteral("[main]: telemetry file \"%1\" does not exist or is not a file. Exiting...")
                .arg(telemetryFileInfo.filePath());

        return 2;
    }

    const std::filesystem::path telemetryFilePath = telemetryFileInfo.filePath().toStdString();
    std::ifstream telemetryFile { telemetryFilePath };

    if (!telemetryFile.is_open())
    {
        qCritical().noquote() << QStringLiteral("[main]: failed to open file \"%1\". Exiting...")
                .arg(QString::fromStdString(telemetryFilePath.string()));

        return 3;
    }

    const Telemetry telemetry = ParseTelemetry(telemetryFile);

    PlotTelemetry(telemetry);

    return QApplication::exec();
}
