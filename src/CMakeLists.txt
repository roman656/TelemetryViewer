set(EXECUTABLE_NAME "${PROJECT_NAME}")

add_subdirectory(Telemetry)

add_executable(${EXECUTABLE_NAME} main.cpp)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS OFF)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        ${PROJECT_NAME}::TelemetryLib
        ${PROJECT_NAME}_ProjectSettings)
