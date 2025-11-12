set(EXECUTABLE_NAME "${PROJECT_NAME}")

add_subdirectory(TelemetryLoader)
add_subdirectory(Core)
add_subdirectory(View)

add_executable(${EXECUTABLE_NAME} main.cpp "${VIEW_RESOURCES_FILEPATH}")

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS OFF
        AUTORCC ON)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        ${PROJECT_NAME}::CoreLib
        ${PROJECT_NAME}_ProjectSettings
        Qt5::Gui
        Qt5::Qml)
