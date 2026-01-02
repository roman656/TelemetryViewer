#include <CLI/CLI.hpp>

#include <TelemetryViewer/Telemetry/TelemetryParser.hpp>

using namespace TelemetryViewer;

namespace
{

void ConfigureCliParser(CLI::App& cliParser)
{
    const std::string appName = "TelemetryViewer";
    const std::string appVersion = appName + " " + TELEMETRY_VIEWER_VERSION;

    cliParser.get_formatter()->column_width(40);
    cliParser.description("TelemetryViewer is a GUI tool for plotting parameter values from telemetry files");
    cliParser.set_version_flag("-v,--version", appVersion, "Print version info and exit");
}

}



int main(int argc, char* argv[])
{
    std::filesystem::path telemetryFilePath;
    CLI::App cliParser;

    ConfigureCliParser(cliParser);

    cliParser.add_option(
            "telemetry_file",
            telemetryFilePath,
            "Path to telemetry file")->required()->check(CLI::ExistingFile);

    CLI11_PARSE(cliParser, argc, argv);

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
