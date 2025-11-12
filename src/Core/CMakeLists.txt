set(CORE_LIB_NAME "CoreLib")
set(CORE_LIB_HEADERS "${CORE_LIB_NAME}_HEADERS")
set(CORE_LIB_SOURCES "${CORE_LIB_NAME}_SOURCES")

file(GLOB_RECURSE CORE_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE CORE_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(${CORE_LIB_NAME} ${CORE_LIB_SOURCES} ${CORE_LIB_HEADERS})
add_library(${PROJECT_NAME}::${CORE_LIB_NAME} ALIAS ${CORE_LIB_NAME})

set_target_properties(${CORE_LIB_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS OFF
        AUTOMOC ON)

target_link_libraries(${CORE_LIB_NAME}
        PUBLIC
                Qt5::Core
        PRIVATE
                ${PROJECT_NAME}::TelemetryLoaderLib
                ${PROJECT_NAME}_ProjectSettings)

target_include_directories(${CORE_LIB_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
