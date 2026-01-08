include(FetchContent)

FetchContent_Declare(
        sciplot
        GIT_REPOSITORY https://github.com/sciplot/sciplot.git
        GIT_TAG 75d813a362e5ef4a43e671ca84517748060f3b25 # v0.3.1 + some fixes
        GIT_SHALLOW ON
        FIND_PACKAGE_ARGS NAMES sciplot)

option(SCIPLOT_BUILD_EXAMPLES OFF)
option(SCIPLOT_BUILD_TESTS OFF)
option(SCIPLOT_BUILD_DOCS OFF)

FetchContent_MakeAvailable(sciplot)

if(NOT TARGET sciplot::sciplot)
    if(TARGET sciplot)
        add_library(sciplot::sciplot ALIAS sciplot)
    else()
        message(FATAL_ERROR
                " sciplot was made available but no usable target exists.\n"
                " Expected one of:\n"
                "   - sciplot::sciplot\n"
                "   - sciplot\n")
    endif()
endif()
