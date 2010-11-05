set(cmake_modules RTCStageUtils.cmake
                  RTCStagePlugin.cmake)

install(FILES ${cmake_modules} DESTINATION share/cmake/Modules
    COMPONENT cmakestuff)

set(PKG_DESC "RT Component for the Stage robot simulator.")
set(PKG_EXTERNAL_DEPS "openrtm-aist stage")
set(pkg_conf_file ${CMAKE_CURRENT_BINARY_DIR}/rtcstage.pc)
configure_file(rtcstage.pc.in ${pkg_conf_file} @ONLY)
install(FILES ${pkg_conf_file}
    DESTINATION ${RTCS_LIB_INSTALL_DIR}/pkgconfig/ COMPONENT pkgconfig)

