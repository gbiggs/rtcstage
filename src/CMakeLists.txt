set(comp_srcs rtcstage.cpp
    actuator_proxy.cpp
    camera_proxy.cpp
    fiducial_proxy.cpp
    gripper_proxy.cpp
    laser_proxy.cpp
    model_proxy.cpp
    position_proxy.cpp
    ranger_proxy.cpp)
set(standalone_srcs standalone.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR})
include_directories(${Stage_INCLUDE_DIRS} ${OPENRTM_INCLUDE_DIRS})
add_definitions(${Stage_DEFINITIONS} ${OPENRTM_DEFINITIONS})

add_library(${PROJECT_NAME_LOWER} ${LIB_TYPE} ${comp_srcs} ${ALL_IDL_SRCS})
add_dependencies(${PROJECT_NAME_LOWER} ALL_IDL_TARGET)
target_link_libraries(${PROJECT_NAME_LOWER} ${Stage_LIBRARIES}
    ${OPENRTM_LIBRARIES})

add_executable(${PROJECT_NAME_LOWER}_standalone ${standalone_srcs})
target_link_libraries(${PROJECT_NAME_LOWER}_standalone ${PROJECT_NAME_LOWER})

install(TARGETS ${PROJECT_NAME_LOWER} ${PROJECT_NAME_LOWER}_standalone
    EXPORT ${PROJECT_NAME_LOWER}
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} COMPONENT component
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT component
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT component)
install(EXPORT ${PROJECT_NAME_LOWER}
    DESTINATION ${LIB_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    FILE ${PROJECT_NAME_LOWER}Depends.cmake)

