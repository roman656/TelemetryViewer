#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <CLI/CLI.hpp>

#include <TelemetryViewer/Telemetry/TelemetryParser.hpp>

using namespace TelemetryViewer;

int main(int argc, char* argv[])
{
    const std::string appName = "TelemetryViewer";
    const std::string appVersion = appName + " " + TELEMETRY_VIEWER_VERSION;
    std::filesystem::path telemetryFilePath;
    CLI::App app;

    app.get_formatter()->column_width(40);
    app.description("TelemetryViewer, a GUI viewer that plots parameter values from telemetry file");
    app.set_version_flag("-v,--version", appVersion, "Print version info and exit");
    app.add_option("telemetry_file", telemetryFilePath, "Path to telemetry file")->required()->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);

    std::ifstream telemetryFile { telemetryFilePath };

    if (!telemetryFile.is_open())
    {
        std::cerr << "[main]: failed to open file \"" << telemetryFilePath.string() << "\". Exiting..." << std::endl;
        return 1;
    }

    const Telemetry telemetry = ParseTelemetry(telemetryFile);

    std::cout << telemetry.metadata.index << '\n';
    std::cout << telemetry.metadata.unit << '\n';
    std::cout << telemetry.metadata.name << std::endl;
}
