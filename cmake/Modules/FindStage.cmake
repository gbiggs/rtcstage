# Find Stage
#
# The following additional directories are searched:
# STAGE_ROOT (CMake variable)
# STAGE_ROOT (Environment variable)
#
# This sets the following variables:
# Stage_FOUND - True if Stage was found.
# Stage_INCLUDE_DIRS - Directories containing the Stage include files.
# Stage_LIBRARIES - Libraries needed to use Stage.
# Stage_DEFINITIONS - Compiler flags for Stage.
# Stage_VERSION - The version of Stage found.
# Stage_VERSION_MAJOR - The major version of Stage found.
# Stage_VERSION_MINOR - The minor version of Stage found.
# Stage_VERSION_REVISION - The revision version of Stage found.
# Stage_VERSION_CANDIDATE - The candidate version of Stage found.

find_package(FLTK REQUIRED)

find_package(PkgConfig)
pkg_check_modules(PC_STAGE QUIET stage)

find_path(Stage_INCLUDE_DIR stage.hh
    HINTS ${STAGE_ROOT}/include $ENV{STAGE_ROOT}/include
    ${PC_STAGE_INCLUDE_DIRS})
find_library(Stage_LIBRARY stage
    HINTS ${STAGE_ROOT}/lib $ENV{STAGE_ROOT}/lib
    ${PC_STAGE_LIBRARY_DIRS})

set(Stage_DEFINITIONS ${PC_STAGE_CFLAGS_OTHER})
set(Stage_INCLUDE_DIRS ${Stage_INCLUDE_DIR} ${FLTK_INCLUDE_DIR})
set(Stage_LIBRARIES ${Stage_LIBRARY} ${FLTK_LIBRARIES})

set(Stage_VERSION "${PC_STAGE_VERSION}")

string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)-?([a-zA-Z0-9]*)"
    Stage_SPLIT_VERSION "${Stage_VERSION}")
set(Stage_VERSION_MAJOR ${CMAKE_MATCH_1})
set(Stage_VERSION_MINOR ${CMAKE_MATCH_2})
set(Stage_VERSION_REVISION ${CMAKE_MATCH_3})
set(Stage_VERSION_CANDIDATE ${CMAKE_MATCH_4})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Stage
    REQUIRED_VARS Stage_INCLUDE_DIRS Stage_LIBRARY
    VERSION_VAR ${Stage_VERSION})

