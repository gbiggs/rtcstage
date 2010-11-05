cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(RTCStage)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

include(${PROJECT_SOURCE_DIR}/cmake/RTCStageUtils.cmake)

# Set version and OS-specific settings
set(RTCS_VERSION 1.0.0 CACHE STRING "RTC:Stage version")
DISSECT_VERSION()
GET_OS_INFO()

# Add an "uninstall" target
CONFIGURE_FILE ("${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET (uninstall "${CMAKE_COMMAND}" -P
    "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake")

# Component source files
set(comp_srcs comp/rtcstage.cpp)
set(standalone_srcs comp/standalone.cpp)
set(proxy_srcs proxies/model_proxy.cpp
               proxies/actuator_proxy.cpp
               proxies/camera_proxy.cpp
               proxies/fiducial_proxy.cpp
               proxies/gripper_proxy.cpp
               proxies/laser_proxy.cpp
               proxies/position_proxy.cpp
               proxies/ranger_proxy.cpp)
set(idls stage_services)

# Compile the built-in IDL files
COMPILE_IDL_FILES(${PROJECT_SOURCE_DIR}/idl ${idls})
FILTER_LIST("${_idl_srcs}" "hh$" _installable_idl_srcs)
install(FILES ${_installable_idl_srcs}
    DESTINATION ${RTCS_INCLUDE_INSTALL_DIR}/idl COMPONENT headers)

# Get necessary dependency information
GET_PKG_CONFIG_INFO(openrtm-aist REQUIRED)
GET_PKG_CONFIG_INFO(stage REQUIRED)

# Component target
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
if(openrtm-aist_INCLUDE_DIRS OR stage_INCLUDE_DIRS)
    include_directories(${openrtm-aist_INCLUDE_DIRS} ${stage_INCLUDE_DIRS})
endif(openrtm-aist_INCLUDE_DIRS OR stage_INCLUDE_DIRS)
if(openrtm-aist_LIBRARY_DIRS OR stage_LIBRARY_DIRS)
    link_directories(${openrtm-aist_LIBRARY_DIRS} ${stage_LIBRARY_DIRS})
endif(openrtm-aist_LIBRARY_DIRS OR stage_LIBRARY_DIRS)
if(openrtm-aist_CFLAGS OR stage_CFLAGS)
    set_source_files_properties(${comp_srcs} ${proxy_srcs}
        PROPERTIES COMPILE_FLAGS "${openrtm-aist_CFLAGS} ${stage_CFLAGS}")
endif(openrtm-aist_CFLAGS OR stage_CFLAGS)

add_library(rtcstage SHARED ${comp_srcs} ${proxy_srcs} ${_idl_srcs})
add_executable(rtcstage_standalone ${standalone_srcs})

target_link_libraries(rtcstage_standalone rtcstage)
if(openrtm-aist_LINK_FLAGS OR stage_LINK_FLAGS)
    set_target_properties(rtcstage PROPERTIES
        LINK_FLAGS "${openrtm-aist_LINK_FLAGS} ${stage_LINK_FLAGS}")
endif(openrtm-aist_LINK_FLAGS OR stage_LINK_FLAGS)
if(openrtm-aist_LINK_LIBS OR stage_LINK_LIBS)
    target_link_libraries(rtcstage ${openrtm-aist_LINK_LIBS} ${stage_LINK_LIBS})
endif(openrtm-aist_LINK_LIBS OR stage_LINK_LIBS)

install(TARGETS rtcstage LIBRARY DESTINATION ${RTCS_LIB_INSTALL_DIR}
    COMPONENT component)
install(TARGETS rtcstage_standalone RUNTIME DESTINATION bin
    COMPONENT component)

# Subdirectories
add_subdirectory(cmake)
add_subdirectory(comp)
add_subdirectory(doc)
add_subdirectory(examples)
add_subdirectory(idl)
add_subdirectory(proxies)

# Package creation
include(InstallRequiredSystemLibraries)
set(RTC_DESCRIPTION "RT-Component for the Stage simulator.")
set(RTC_VENDOR "Geoffrey Biggs, AIST")
set(RTC_EXECUTABLES "rtc_stage_standalone" "RTC:Stage")
configure_file("${PROJECT_SOURCE_DIR}/cmake/cpack_options.cmake.in"
    "${PROJECT_BINARY_DIR}/cpack_options.cmake" @ONLY)
set(CPACK_PROJECT_CONFIG_FILE "${PROJECT_BINARY_DIR}/cpack_options.cmake")
include(CPack)

