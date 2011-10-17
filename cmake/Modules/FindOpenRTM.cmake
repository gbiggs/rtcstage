# Find OpenRTM-aist
#
# The following additional directories are searched:
# OPENRTM_ROOT (CMake variable)
# OPENRTM_ROOT (Environment variable)
#
# This sets the following variables:
# OPENRTM_FOUND - True if OpenRTM-aist was found.
# OPENRTM_INCLUDE_DIRS - Directories containing the OpenRTM-aist include files.
# OPENRTM_LIBRARIES - Libraries needed to use OpenRTM-aist.
# OPENRTM_DEFINITIONS - Compiler flags for OpenRTM-aist.
# OPENRTM_VERSION - The version of OpenRTM-aist found.
# OPENRTM_VERSION_MAJOR - The major version of OpenRTM-aist found.
# OPENRTM_VERSION_MINOR - The minor version of OpenRTM-aist found.
# OPENRTM_VERSION_REVISION - The revision version of OpenRTM-aist found.
# OPENRTM_VERSION_CANDIDATE - The candidate version of OpenRTM-aist found.
# OPENRTM_IDL_COMPILER - The IDL compiler used by OpenRTM-aist.
# OPENRTM_IDL_FLAGS - The flags used to compile OpenRTM-aist IDL files.
# OPENRTM_IDL_DIR - The directory containing the OpenRTM-aist IDL files.
#
# This module also defines one macro usable in your CMakeLists.txt files:
# OPENRTM_COMPILE_IDL_FILES(output_dir file1 file2 ...)
#   Compiles the specified IDL files, placing the generated C++ source files in
#   ${output_dir}. The source files can be found in file1_SOURCES and
#   file1_HEADERS,
#   file2_SOURCES and file2_HEADERS, etc., all source files for all IDL files are
#   available in IDL_ALL_SOURCES, and all header files can be found in
#   IDL_ALL_HEADERS. To depend on the generated files, depend on the targets
#   file1_TGT, file2_TGT, etc. The target IDL_ALL_TARGET is available to depend
#   on all source files generated from IDL files.

find_package(PkgConfig)
pkg_check_modules(PC_OPENRTM QUIET openrtm-aist)
pkg_check_modules(PC_COIL QUIET libcoil)

find_path(OPENRTM_INCLUDE_DIR rtm/RTC.h
    HINTS ${OPENRTM_ROOT}/include $ENV{OPENRTM_ROOT}/include
    ${PC_OPENRTM_INCLUDE_DIRS})
find_path(COIL_INCLUDE_DIR coil/config_coil.h
    HINTS ${OPENRTM_ROOT}/include $ENV{OPENRTM_ROOT}/include
    ${PC_COIL_INCLUDE_DIRS})
find_library(OPENRTM_LIBRARY RTC
    HINTS ${OPENRTM_ROOT}/lib $ENV{OPENRTM_ROOT}/lib
    ${PC_OPENRTM_LIBRARY_DIRS})
find_library(COIL_LIBRARY coil
    HINTS ${OPENRTM_ROOT}/lib $ENV{OPENRTM_ROOT}/lib
    ${PC_COIL_LIBRARY_DIRS})

set(OPENRTM_DEFINITIONS ${PC_OPENRTM_CFLAGS_OTHER} ${PC_COIL_CFLAGS_OTHER})
set(OPENRTM_INCLUDE_DIRS ${OPENRTM_INCLUDE_DIR} ${OPENRTM_INCLUDE_DIR}/rtm/idl
    ${COIL_INCLUDE_DIR})
set(OPENRTM_LIBRARIES ${OPENRTM_LIBRARY} ${COIL_LIBRARY} uuid dl pthread
    omniORB4 omnithread omniDynamic4)

file(STRINGS ${OPENRTM_INCLUDE_DIR}/rtm/version.h OPENRTM_VERSION
    NEWLINE_CONSUME)
#set(OPENRTM_VERSION "1.1.0")
string(REGEX MATCH "version = \"([0-9]+)\\.([0-9]+)\\.([0-9]+)-?([a-zA-Z0-9]*)\""
    OPENRTM_VERSION "${OPENRTM_VERSION}")
set(OPENRTM_VERSION_MAJOR ${CMAKE_MATCH_1})
set(OPENRTM_VERSION_MINOR ${CMAKE_MATCH_2})
set(OPENRTM_VERSION_REVISION ${CMAKE_MATCH_3})
set(OPENRTM_VERSION_CANDIDATE ${CMAKE_MATCH_4})

execute_process(COMMAND rtm-config --idlc OUTPUT_VARIABLE OPENRTM_IDL_COMPILER
    OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND rtm-config --idlflags OUTPUT_VARIABLE OPENRTM_IDL_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
separate_arguments(OPENRTM_IDL_FLAGS)
execute_process(COMMAND rtm-config --prefix OUTPUT_VARIABLE _rtm_prefix
    OUTPUT_STRIP_TRAILING_WHITESPACE)
set(OPENRTM_IDL_DIR
    ${_rtm_prefix}/include/openrtm-${OPENRTM_VERSION_MAJOR}.${OPENRTM_VERSION_MINOR}/rtm/idl
    CACHE STRING "Directory containing the OpenRTM-aist IDL files.")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenRTM
    REQUIRED_VARS OPENRTM_INCLUDE_DIR COIL_INCLUDE_DIR OPENRTM_LIBRARY
    COIL_LIBRARY OPENRTM_IDL_COMPILER VERSION_VAR ${OPENRTM_VERSION})


###############################################################################
# Figure out the file names of the source outputs from omniidl.
macro(_idl_source_outputs _result _idl _dir)
    set(${_result} ${_dir}/${_idl}SK.cc ${_dir}/${_idl}DynSK.cc)
endmacro(_idl_source_outputs)


###############################################################################
# Figure out the file names of the header outputs from omniidl.
macro(_idl_header_output _result _idl _dir)
    set(${_result} ${_dir}/${_idl}.hh)
endmacro(_idl_header_output)


###############################################################################
# Compile an IDL file, placing the output in the specified directory.
# The output header file names will be placed in ${_idl}_HEADERS and appended
# to IDL_ALL_HEADERS. The output source file names will be placed in
# ${_idl}_SOURCES and appended to IDL_ALL_SOURCES.
macro(_compile_intf_idl _idl_file _dir)
    get_filename_component(_idl "${_idl_file}" NAME_WE)
    execute_process(COMMAND rtm-config --idlc OUTPUT_VARIABLE _idl_compiler
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(NOT _idl_compiler)
        message(FATAL_ERROR "Could not find IDL compiler.")
    endif(NOT _idl_compiler)
    execute_process(COMMAND rtm-config --idlflags OUTPUT_VARIABLE _idlc_flags
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    separate_arguments(_idlc_flags)
    execute_process(COMMAND rtm-config --prefix OUTPUT_VARIABLE _rtm_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(_idl_srcs_var ${_idl}_SOURCES)
    _idl_source_outputs(${_idl_srcs_var} ${_idl} ${_dir})
    set(_idl_hdrs_var ${_idl}_HEADERS)
    _idl_header_output(${_idl_hdrs_var} ${_idl} ${_dir})
    file(MAKE_DIRECTORY ${_dir})
    add_custom_command(OUTPUT ${${_idl_srcs_var}} ${${_idl_hdrs_var}}
        COMMAND ${OPENRTM_IDL_COMPILER} ${OPENRTM_IDL_FLAGS}
        -I${OPENRTM_IDL_DIR} -C${_dir} ${_idl_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${_idl_file}
        COMMENT "Compiling ${_idl_file} for CORBA" VERBATIM)
    set(IDL_ALL_SOURCES ${IDL_ALL_SOURCES} ${${_idl_srcs_var}})
    set(IDL_ALL_HEADERS ${IDL_ALL_HEADERS} ${${_idl_hdrs_var}})
    add_custom_target(${_idl}_TGT DEPENDS ${${_idl_srcs_var}})
    if(NOT TARGET IDL_ALL_TARGET)
        add_custom_target(IDL_ALL_TARGET)
    endif(NOT TARGET IDL_ALL_TARGET)
    add_dependencies(IDL_ALL_TARGET ${_idl}_TGT)
endmacro(_compile_intf_idl)


###############################################################################
# Compile a set of IDL files, placing the outputs in the location
# specified by _output_dir.
macro(OPENRTM_COMPILE_IDL_FILES _output_dir)
    set(IDL_ALL_SOURCES)
    set(IDL_ALL_HEADERS)
    foreach(idl ${ARGN})
        _compile_intf_idl(${idl} ${_output_dir})
    endforeach(idl)
    set(IDL_HEADERS_DIR ${_output_dir})
endmacro(OPENRTM_COMPILE_IDL_FILES)

