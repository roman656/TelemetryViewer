include(FetchContent)

FetchContent_Declare(
        qcustomplot
        URL https://www.qcustomplot.com/release/2.1.1/QCustomPlot-source.tar.gz)

FetchContent_MakeAvailable(qcustomplot)

add_library(qcustomplot STATIC
        ${qcustomplot_SOURCE_DIR}/qcustomplot.cpp
        ${qcustomplot_SOURCE_DIR}/qcustomplot.h)
add_library(QCustomPlot::QCustomPlot ALIAS qcustomplot)

set_target_properties(qcustomplot PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS OFF
        AUTOMOC ON)

target_link_libraries(qcustomplot PUBLIC Qt6::Widgets Qt6::PrintSupport)
target_include_directories(qcustomplot PUBLIC ${qcustomplot_SOURCE_DIR})
