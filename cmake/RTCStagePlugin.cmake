cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

if(COMMAND CMAKE_POLICY)
    if(POLICY CMP0011)
        CMAKE_POLICY(SET CMP0011 NEW)
    endif(POLICY CMP0011)
endif(COMMAND CMAKE_POLICY)

include(RTCStageUtils)

GET_PKG_CONFIG_INFO(rtcstage REQUIRED)
set(_int_inc_dirs ${rtcstage_INCLUDE_DIRS})
set(_int_lib_dirs ${rtcstage_LIBRARY_DIRS})
set(_int_link_flags ${rtcstage_LINK_FLAGS})
set(_int_link_libs ${rtcstage_LINK_LIBS})
set(_int_cflags ${rtcstage_CFLAGS})

###############################################################################
# Macro to build a model proxy plugin for the RTC:Stage RT Component.
# _plugin_name: The name of the plugin to create.
# Pass source files, idl files, flags, etc. as extra args preceeded by keywords
# as follows:
# SOURCES <source files list>
# INCLUDEDIRS <include directories list>
# LIBDIRS <library directories list>
# LINKLIBS <list of libraries to link to>
# LINKFLAGS <link flags list>
# CFLAGS <compile flags list>
# IDL <idl files list>
macro(BUILD_PROXY_PLUGIN _plugin_name)
    PROCESS_ARGUMENTS(_srcs _include_dirs _lib_dirs _link_libs _link_flags
        _cflags _idl _junk ${ARGN})
    if(_junk)
        message(STATUS "WARNING: unkeyworded arguments found in BuildProxyPlugin: ${_junk}")
    endif(_junk)
    LIST_TO_STRING(_cFlags "${_cFlags}")

    if(_idl)
        include_directories(${PROJECT_BINARY_DIR})
        COMPILE_IDL_FILES(${PROJECT_SOURCE_DIR} ${_idl})
    endif(_idl)

    if(_include_dirs OR _int_inc_dirs)
        include_directories(${_include_dirs} ${_int_inc_dirs})
    endif(_include_dirs OR _int_inc_dirs)
    if(_lib_dirs OR _int_lib_dirs)
        link_directories(${_lib_dirs} ${_int_lib_dirs})
    endif(_lib_dirs OR _int_lib_dirs)

    add_library(${_plugin_name} SHARED ${_srcs} ${_idl_srcs})

    if(_int_link_flags)
        set_target_properties(${_plugin_name} PROPERTIES
            LINK_FLAGS "${_int_link_flags}")
    endif(_int_link_flags)
    if(_link_flags)
        LIST_TO_STRING(_link_flags "${_link_flags}")
        set_target_properties(${_plugin_name} PROPERTIES
            LINK_FLAGS ${_link_flags})
    endif(_link_flags)
    if(_link_libs)
        target_link_libraries(${_plugin_name} ${_link_libs})
    endif(_link_libs)
    if(_int_link_libs)
        target_link_libraries(${_plugin_name} ${_int_link_libs})
    endif(_int_link_libs)

    # Get the current cflags for each source file, and add the global ones
    # (this allows the user to specify individual cflags for each source file
    # without the global ones overriding them).
    if(_cflags OR _int_cflags)
        foreach(_file ${_srcs})
            get_source_file_property(_file_cflags ${_file} COMPILE_FLAGS)
            if(_file_cflags STREQUAL NOTFOUND)
                set(_new_cflags "${_int_cflags} ${_cflags}")
            else(_file_cflags STREQUAL NOTFOUND)
                set(_new_cflags "${_file_cflags} ${_int_cflags} ${_cflags}")
            endif(_file_cflags STREQUAL NOTFOUND)
            set_source_files_properties(${_file} PROPERTIES
                COMPILE_FLAGS ${_new_cflags})
        endforeach(_file)
    endif(_cflags OR _int_cflags)
endmacro(BUILD_PROXY_PLUGIN)

