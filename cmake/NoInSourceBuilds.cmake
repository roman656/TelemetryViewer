if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR
            " In-source builds are not allowed.\n"
            " Please create a separate build directory outside the source tree.\n"
            " To fix this, remove any temporary files and directories created by CMake in the source tree.\n"
            " Then create a clean build directory, e.g.:\n"
            "   cmake -S . -B build\n"
            "   cmake --build build")
endif()
