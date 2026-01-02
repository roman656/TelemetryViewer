cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(TelemetryViewer
        VERSION 0.1.0
        LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(NoInSourceBuilds)
include(ProjectSettings)

add_subdirectory(src)
