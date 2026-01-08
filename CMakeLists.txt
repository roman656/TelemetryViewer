cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(TelemetryViewer
        VERSION 0.1.0
        DESCRIPTION "GUI tool for telemetry plotting"
        LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(NoInSourceBuilds)
include(ProjectSettings)

find_package(Qt6 COMPONENTS Widgets PrintSupport REQUIRED)

add_subdirectory(src)
