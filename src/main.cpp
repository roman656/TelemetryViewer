#include <fstream>
#include <iostream>

#include <TelemetryViewer/Telemetry/TelemetryParser.hpp>

using namespace TelemetryViewer;

int main(int argc, char* argv[])
{
    const std::string telemetryFilename = "Data.txt";
    std::ifstream telemetryFile { telemetryFilename };

    if (!telemetryFile.is_open())
    {
        std::cerr << "[main]: failed to open file \"" << telemetryFilename << "\". Exiting..." << std::endl;
        return 1;
    }

    const Telemetry telemetry = ParseTelemetry(telemetryFile);

    std::cout << telemetry.metadata.index << '\n';
    std::cout << telemetry.metadata.unit << '\n';
    std::cout << telemetry.metadata.name << std::endl;
}
