set(EXECUTABLE_NAME "${PROJECT_NAME}")

add_subdirectory(Telemetry)
add_subdirectory(TelemetryPlotting)

add_executable(${EXECUTABLE_NAME} main.cpp)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS OFF)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        ${PROJECT_NAME}::TelemetryPlottingLib
        ${PROJECT_NAME}_ProjectSettings
        Qt6::Widgets)

target_compile_definitions(${EXECUTABLE_NAME} PRIVATE TELEMETRY_VIEWER_VERSION="${PROJECT_VERSION}")
