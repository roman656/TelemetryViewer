cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(TelemetryViewer
        VERSION 0.0.1
        LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(NoInSourceBuilds)
include(ProjectSettings)

find_package(Qt5 COMPONENTS Quick Charts REQUIRED)

add_subdirectory(src)
